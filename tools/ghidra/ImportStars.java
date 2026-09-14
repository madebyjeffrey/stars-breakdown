// Import recovered CodeView metadata. Run before automatic analysis.
// @category Stars
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;
import java.math.BigInteger;
import ghidra.program.model.mem.*;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.pcode.JumpTable;
public class ImportStars extends GhidraScript {
 Path root; Map<Integer,JsonObject> records=new HashMap<>(); Map<Integer,DataType> types=new HashMap<>();
 Map<String,JsonObject> symbols=new HashMap<>(); JsonArray issues=new JsonArray(); CategoryPath cat=new CategoryPath("/Stars/CodeView");
 JsonArray read(String n)throws Exception{return JsonParser.parseString(Files.readString(root.resolve("analysis/debug/"+n+".json"))).getAsJsonArray();}
 int num(JsonObject o,String k){return o.get(k).getAsInt();} String str(JsonObject o,String k){return o.get(k).getAsString();}
 Address addr(JsonObject r){return toAddr(String.format("%04x:%04x",0x1000+8*(num(r,"ne_segment")-1),num(r,"ne_offset")));}
 void issue(String what,Exception e){JsonObject j=new JsonObject();j.addProperty("item",what);j.addProperty("error",e.toString());issues.add(j);}
 DataType type(int id){
  if(types.containsKey(id))return types.get(id);
  JsonObject r=records.get(id); DataType d=Undefined1DataType.dataType;
  if(id<4096){int b=id&255;int mode=id>>8;
   if(mode!=0)d=new PointerDataType(type(b),mode==1?2:4,currentProgram.getDataTypeManager());
   else if(b==3)d=VoidDataType.dataType;
   else if(b==0x40)d=FloatDataType.dataType;
   else if(b==0x41)d=DoubleDataType.dataType;
   else if(b==0x42)d=Float10DataType.dataType;
   else {boolean u=(b>=0x20&&b<0x30)||b==0x73;int size=(b==0x12||b==0x22)?4:(b==0x10||b==0x20||b==0x70)?1:2;
    d=size==1?(u?UnsignedCharDataType.dataType:SignedCharDataType.dataType):size==4?(u?UnsignedLongDataType.dataType:LongDataType.dataType):(u?UnsignedShortDataType.dataType:ShortDataType.dataType);}
  }else if(r!=null){String k=str(r,"kind");
   if(k.equals("LF_POINTER16"))d=new PointerDataType(type(num(r,"underlying_type")),num(r,"size_bytes"),currentProgram.getDataTypeManager());
   else if(k.equals("LF_MODIFIER16")||k.equals("LF_BITFIELD16"))d=type(num(r,"underlying_type"));
   else if(k.equals("LF_ARRAY16")){DataType el=type(num(r,"element_type")); d=new ArrayDataType(el,num(r,"element_count"),el.getLength(),currentProgram.getDataTypeManager());}
   else if(k.equals("LF_PROCEDURE16")){FunctionDefinitionDataType f=new FunctionDefinitionDataType(cat,"cv_"+Integer.toHexString(id),currentProgram.getDataTypeManager());types.put(id,f);f.setReturnType(type(num(r,"return_type"))); ArrayList<ParameterDefinition> p=new ArrayList<>();int n=0;for(JsonElement a:r.getAsJsonArray("argument_types")){if(a.getAsInt()!=0)p.add(new ParameterDefinitionImpl("arg"+n++,type(a.getAsInt()),null));}f.setArguments(p.toArray(new ParameterDefinition[0]));f.setVarArgs(r.get("variadic").getAsBoolean());d=f;}
  }
  types.put(id,d);return d;
 }
 public void run()throws Exception{
  root=Path.of(getScriptArgs()[0]);
  setAnalysisOption(currentProgram,"Apply Data Archives","false");
  setAnalysisOption(currentProgram,"Call Convention ID","false");
  setAnalysisOption(currentProgram,"Stack.Create Local Variables","false");
  JsonObject layout=JsonParser.parseString(Files.readString(root.resolve("analysis/ne/layout.json"))).getAsJsonObject();
  for(JsonElement e:layout.getAsJsonArray("relocations")){JsonObject r=e.getAsJsonObject();if(!r.has("import_key"))continue;String key=str(r,"import_key");int value=key.equals("KERNEL!#113")?3:key.equals("KERNEL!#114")?8:-1;if(value<0)continue;
   for(JsonElement site:r.getAsJsonArray("sites")){Address a=toAddr(String.format("%04x:%04x",0x1000+8*(num(r,"segment")-1),site.getAsInt()));currentProgram.getMemory().setShort(a,(short)value);setEOLComment(a,"Kernel absolute export "+key+" = "+value+" in the documented protected-mode selector model.");}
  }
  createData(toAddr("1118:0010"),UnsignedShortDataType.dataType);
  MutabilitySettingsDefinition.DEF.setChoice(getDataAt(toAddr("1118:0010")),MutabilitySettingsDefinition.VOLATILE);
  setPlateComment(toAddr("1118:0010"),"__wflags data initialized from KERNEL.__WINFLAGS at load time; placeholder bytes are not an observed Windows flag value.");
  for(MemoryBlock b:currentProgram.getMemory().getBlocks())if(b.getName().startsWith("Code")){
   int sel=0x1000+8*(Integer.parseInt(b.getName().substring(4))-1);
   currentProgram.getProgramContext().setValue(currentProgram.getRegister("CS"),b.getStart(),b.getEnd(),BigInteger.valueOf(sel));
  }
  for(JsonElement e:read("types")){JsonObject r=e.getAsJsonObject();int id=num(r,"index");records.put(id,r);if(str(r,"kind").equals("LF_STRUCTURE16"))types.put(id,new StructureDataType(cat,str(r,"name"),num(r,"size_bytes"),currentProgram.getDataTypeManager()));}
  for(JsonElement e:read("symbols")){JsonObject r=e.getAsJsonObject();symbols.put(str(r,"id"),r);}
  for(JsonObject r:records.values())if(str(r,"kind").equals("LF_STRUCTURE16")){
   Structure s=(Structure)types.get(num(r,"index"));ArrayList<JsonObject> fields=new ArrayList<>();for(JsonElement e:r.getAsJsonArray("members"))if(num(e.getAsJsonObject(),"size_bytes")>0)fields.add(e.getAsJsonObject());fields.sort(Comparator.comparingInt(x->num(x,"offset")));
   for(int i=0;i<fields.size();){int start=num(fields.get(i),"offset"),end=start+num(fields.get(i),"size_bytes"),j=i+1;while(j<fields.size()&&num(fields.get(j),"offset")<end){end=Math.max(end,num(fields.get(j),"offset")+num(fields.get(j),"size_bytes"));j++;}
    try{if(j==i+1&&!fields.get(i).has("bit_length")){JsonObject f=fields.get(i);s.replaceAtOffset(start,type(num(f,"type_index")),num(f,"size_bytes"),str(f,"name"),null);}
    else{UnionDataType u=new UnionDataType(cat,s.getName()+"_at_"+start,currentProgram.getDataTypeManager());for(int n=i;n<j;n++){JsonObject f=fields.get(n);StructureDataType v=new StructureDataType(cat,u.getName()+"_"+str(f,"name"),end-start,currentProgram.getDataTypeManager());if(f.has("bit_length"))v.insertBitFieldAt(num(f,"offset")-start,num(f,"size_bytes"),num(f,"bit_position"),type(num(f,"type_index")),num(f,"bit_length"),str(f,"name"),null);else v.replaceAtOffset(num(f,"offset")-start,type(num(f,"type_index")),num(f,"size_bytes"),str(f,"name"),null);u.add(v,str(f,"name")+"_view",null);}s.replaceAtOffset(start,u,end-start,"overlap_"+start,"All overlapping debug fields retained as views.");}
    }catch(Exception ex){issue(s.getName()+" field "+start,ex);}i=j;
   }
  }
  for(Integer id:records.keySet())try{type(id);}catch(Exception e){issue("type "+id,e);}
  for(DataType t:new ArrayList<>(types.values()))currentProgram.getDataTypeManager().resolve(t,DataTypeConflictHandler.DEFAULT_HANDLER);
  for(JsonElement e:read("aliases")){JsonObject a=e.getAsJsonObject();currentProgram.getDataTypeManager().resolve(new TypedefDataType(new CategoryPath("/Stars/Aliases"),str(a,"name"),type(num(a,"type_index")),currentProgram.getDataTypeManager()),DataTypeConflictHandler.DEFAULT_HANDLER);}
  for(JsonElement e:read("globals")){JsonObject r=e.getAsJsonObject();try{Address a=addr(r);createLabel(a,str(r,"name"),true);DataType d=type(num(r,"type_index"));if(d.getLength()>0&&getDataAt(a)==null)createData(a,d);}catch(Exception ex){issue("global "+str(r,"name"),ex);}}
  for(JsonElement e:JsonParser.parseString(Files.readString(root.resolve("references/win16_abi.json"))).getAsJsonArray()){
   JsonObject r=e.getAsJsonObject();if(!r.has("arguments")||r.get("flags").toString().contains("-register"))continue;
   try{Function thunk=getFunctionAt(toAddr(str(r,"address")));Function f=thunk.getThunkedFunction(true);boolean pascal=str(r,"convention").equals("pascal");
    ArrayList<DataType> at=new ArrayList<>();int total=0;for(JsonElement a:r.getAsJsonArray("arguments")){String t=a.getAsString();DataType d=t.equals("word")?type(115):t.equals("s_word")?type(114):t.equals("long")?type(18):new PointerDataType(t.contains("str")?type(112):VoidDataType.dataType,4,currentProgram.getDataTypeManager());at.add(d);total+=d.getLength();}
    ArrayList<Variable> params=new ArrayList<>();int off=pascal?total+4:4;for(int i=0;i<at.size();i++){DataType d=at.get(i);if(pascal)off-=d.getLength();params.add(new ParameterImpl("arg"+i,d,off,currentProgram));if(!pascal)off+=d.getLength();}
    String conv=pascal?"__stdcall16far":"__cdecl16far";DataType rt=type(num(r,"return_bytes")==2?114:18);
    f.updateFunction(conv,new ReturnParameterImpl(rt,currentProgram),Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,true,SourceType.IMPORTED,new Variable[0]);
    f.updateFunction(conv,new ReturnParameterImpl(rt,f.getReturn().getVariableStorage(),currentProgram),Function.FunctionUpdateType.CUSTOM_STORAGE,true,SourceType.IMPORTED,params.toArray(new Variable[0]));f.setStackPurgeSize(pascal?total:0);f.setVarArgs(str(r,"convention").equals("varargs"));f.setComment("ABI widths/order from pinned Wine Win16 specification; return signedness and semantic typedefs not recovered from spec.");
   }catch(Exception ex){issue("import "+str(r,"name"),ex);}
  }
  for(JsonElement e:read("procedures")){
   monitor.checkCancelled();JsonObject r=e.getAsJsonObject();String name=str(r,"name");try{
    Address a=addr(r);AddressSet extent=new AddressSet(a,a.add(num(r,"size")-1));new DisassembleCommand(a,extent,true).applyTo(currentProgram,monitor);
    Function f=getFunctionAt(a);if(f==null)f=currentProgram.getFunctionManager().createFunction(name,a,extent,SourceType.IMPORTED);else{f.setName(name,SourceType.IMPORTED);f.setBody(extent);}
    JsonObject sig=records.get(num(r,"type_index"));String cv=str(sig,"calling_convention_name");String conv="__"+(cv.contains("pascal")?"stdcall":"cdecl")+"16"+(cv.contains("far")?"far":"near");
    ArrayList<Variable> args=new ArrayList<>();JsonArray ids=r.getAsJsonArray("parameter_symbols"),pt=r.getAsJsonArray("effective_parameter_types"),pn=r.getAsJsonArray("effective_parameter_names");int stack=cv.contains("far")?4:2;
    for(int k=0;k<pt.size();k++){DataType d=type(pt.get(k).getAsInt());int off=stack;if(k<ids.size()){JsonObject v=symbols.get(ids.get(k).getAsString());if(v.has("bp_offset"))off=num(v,"bp_offset")-2;}if(name.equals("FrameWndProc"))off=new int[]{12,10,8,4}[k];args.add(new ParameterImpl(pn.get(k).getAsString(),d,off,currentProgram));stack+=Math.max(2,d.getLength());}
    // Assign the standard return registers first, then retain explicit Pascal/debug parameter storage.
    f.updateFunction(conv,new ReturnParameterImpl(type(num(sig,"return_type")),currentProgram),Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,true,SourceType.IMPORTED,new Variable[0]);
    Variable ret=new ReturnParameterImpl(type(num(sig,"return_type")),f.getReturn().getVariableStorage(),currentProgram);
    f.updateFunction(conv,ret,Function.FunctionUpdateType.CUSTOM_STORAGE,true,SourceType.IMPORTED,args.toArray(new Variable[0]));
    f.setVarArgs(sig.get("variadic").getAsBoolean());f.setStackPurgeSize(cv.contains("pascal")?stack-(cv.contains("far")?4:2):0);
    f.setComment("CodeView "+str(r,"id")+"; NE "+num(r,"ne_segment")+":"+Integer.toHexString(num(r,"ne_offset"))+"; "+r.get("source_files")+"\n"+str(r,"signature_resolution")+"\nLocals and lexical lifetimes: analysis/debug/symbols.json");
   }catch(Exception ex){issue("procedure "+name,ex);}
  }
  // Public aliases also expose runtime entry points absent from S_PROC16 records.
  for(JsonObject r:symbols.values())if(str(r,"kind").equals("S_PUB16")&&r.has("ne_segment")){
   try{Address a=addr(r);String name=str(r,"name");createLabel(a,name,false);
    if(num(r,"ne_segment")<=36&&!name.equals("__wflags")){
     Function f=getFunctionAt(a);if(f==null&&getDataContaining(a)==null){disassemble(a);if(getInstructionAt(a)!=null)f=createFunction(a,name);}
     if(f!=null&&(f.getName().startsWith("FUN_")||f.getName().startsWith("caseD_")))f.setName(name,SourceType.IMPORTED);
    }
   }catch(Exception ex){issue("public "+str(r,"name"),ex);}
  }
  // These shifts have directly checked DX:AX value / CL count register interfaces.
  for(int offset:new int[]{0xd50,0xd5c,0xe32}){Function f=getFunctionAt(toAddr(String.format("1118:%04x",offset)));if(f!=null){
   VariableStorage pair=new VariableStorage(currentProgram,new Varnode(currentProgram.getRegister("DX").getAddress(),2),new Varnode(currentProgram.getRegister("AX").getAddress(),2));
   Variable v=new ParameterImpl("value",type(18),pair,currentProgram);Variable count=new ParameterImpl("count",type(32),new VariableStorage(currentProgram,currentProgram.getRegister("CL")),currentProgram);
   f.updateFunction("__cdecl16far",new ReturnParameterImpl(type(18),pair,currentProgram),Function.FunctionUpdateType.CUSTOM_STORAGE,true,SourceType.IMPORTED,v,count);f.setStackPurgeSize(0);
  }}
  for(int off:new int[]{0xbe4,0xc7e,0xcb0,0xd68,0xdc8}){Function f=getFunctionAt(toAddr(String.format("1118:%04x",off)));DataType d=off>=0xd68?type(34):type(18);
   f.updateFunction("__stdcall16far",new ReturnParameterImpl(d,currentProgram),Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,true,SourceType.IMPORTED,new ParameterImpl("left",d,4,currentProgram),new ParameterImpl("right",d,8,currentProgram));f.setStackPurgeSize(8);
  }
  // Conservative decompiler workaround for an address-tied far-pointer merge failure.
  // This is not a recovered C volatile qualifier. Original type stays in debug JSON/headers.
  MutabilitySettingsDefinition.DEF.setChoice(getDataAt(toAddr("1120:00f6")),MutabilitySettingsDefinition.VOLATILE);
  JsonObject dispatch=JsonParser.parseString(Files.readString(root.resolve("analysis/decompiler/runtime-dispatch.json"))).getAsJsonObject();
  for(String key:new String[]{"error_dispatch","math_dispatch"}){
   JsonObject r=dispatch.getAsJsonObject(key);Address a=toAddr(str(r,"instruction"));ArrayList<Address> targets=new ArrayList<>();for(JsonElement e:r.getAsJsonArray("targets")){Address t=toAddr(e.getAsString());targets.add(t);currentProgram.getReferenceManager().addMemoryReference(a,t,RefType.COMPUTED_JUMP,SourceType.USER_DEFINED,0);disassemble(t);}
   Function f=getFunctionContaining(a);if(f==null)throw new Exception("No runtime dispatch owner at "+a);new JumpTable(a,targets,true,0).writeOverride(f);
  }
  Map<Address,StringBuilder> lines=new TreeMap<>();for(JsonElement e:JsonParser.parseString(Files.readString(root.resolve("analysis/source_lines.json"))).getAsJsonArray()){
   JsonObject r=e.getAsJsonObject();int seg=num(r,"segment");if(seg>36)continue;Address a=toAddr(String.format("%04x:%04x",0x1000+8*(seg-1),num(r,"offset")));lines.computeIfAbsent(a,k->new StringBuilder()).append(str(r,"file")).append(':').append(num(r,"line")).append('\n');
  }for(Map.Entry<Address,StringBuilder> e:lines.entrySet())setEOLComment(e.getKey(),e.getValue().toString());
  Files.writeString(root.resolve("analysis/decompiler/import-issues.json"),new GsonBuilder().setPrettyPrinting().create().toJson(issues));println("Metadata import issues: "+issues.size());
 }
}
