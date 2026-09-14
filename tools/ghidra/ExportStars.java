// Export raw analysis. This is evidence, not reviewed reconstruction.
// @category Stars
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.pcode.*;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;
public class ExportStars extends GhidraScript {
 public void run()throws Exception{
  Path out=Path.of(getScriptArgs()[0]);Files.createDirectories(out.resolve("raw"));Files.createDirectories(out.resolve("memory"));
  boolean samples=getScriptArgs().length>1;Set<String> wanted=Set.of("GenerateWorld","LInnateRaceHabitability","GetRaceStat","GetRaceGrbit","GetFuelFree","ICompLong","AskSaveDialog","FrameWndProc","LpengineFromId","SetFileSeeds","ReadIniSettings");
  DecompInterface dec=new DecompInterface();dec.openProgram(currentProgram);JsonArray functions=new JsonArray(),refs=new JsonArray(),indirect=new JsonArray(),tables=new JsonArray();
  for(MemoryBlock b:currentProgram.getMemory().getBlocks())if(b.getName().matches("(Code|Data)[0-9]+")){byte[] bytes=new byte[(int)b.getSize()];b.getBytes(b.getStart(),bytes);Files.write(out.resolve("memory/"+b.getName()+".bin"),bytes);}
  FunctionIterator it=currentProgram.getFunctionManager().getFunctions(true);int n=0;
  while(it.hasNext()){monitor.checkCancelled();Function f=it.next();if(f.isExternal()||currentProgram.getMemory().getBlock(f.getEntryPoint()).isArtificial()||samples&&!wanted.contains(f.getName()))continue;
   String key=f.getEntryPoint().toString().replace(':','_');JsonObject j=new JsonObject();j.addProperty("address",f.getEntryPoint().toString());j.addProperty("name",f.getName());j.addProperty("signature",f.getPrototypeString(false,false));j.addProperty("convention",f.getCallingConventionName());j.addProperty("purge",f.getStackPurgeSize());j.addProperty("body_bytes",f.getBody().getNumAddresses());j.addProperty("thunk",f.isThunk());j.addProperty("return_bytes",f.getReturnType().getLength());j.addProperty("return_storage",f.getReturn().getVariableStorage().toString());JsonArray params=new JsonArray();for(Parameter p:f.getParameters()){JsonObject q=new JsonObject();q.addProperty("name",p.getName());q.addProperty("type",p.getDataType().getName());q.addProperty("size",p.getLength());q.addProperty("storage",p.getVariableStorage().toString());params.add(q);}j.add("parameters",params);JsonArray ranges=new JsonArray();for(AddressRange r:f.getBody()){JsonObject q=new JsonObject();q.addProperty("start",r.getMinAddress().toString());q.addProperty("end",r.getMaxAddress().toString());ranges.add(q);}j.add("ranges",ranges);
   DecompileResults result=dec.decompileFunction(f,60,monitor);j.addProperty("decompiled",result.decompileCompleted());j.addProperty("message",result.getErrorMessage());
   if(result.decompileCompleted())Files.writeString(out.resolve("raw/"+key+"_"+f.getName().replaceAll("[^a-zA-Z0-9_]","_")+".c"),"/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */\n"+result.getDecompiledFunction().getC());
   if(result.decompileCompleted())for(JumpTable t:result.getHighFunction().getJumpTables()){JsonObject q=new JsonObject();q.addProperty("function",f.getName());q.addProperty("address",t.getSwitchAddress().toString());JsonArray cases=new JsonArray();for(Address a:t.getCases())cases.add(a.toString());q.add("targets",cases);tables.add(q);}
   StringBuilder asm=new StringBuilder();InstructionIterator ins=currentProgram.getListing().getInstructions(f.getBody(),true);int count=0;
   while(ins.hasNext()){Instruction in=ins.next();count++;asm.append(in.getAddress()).append("  ");for(byte b:in.getBytes())asm.append(String.format("%02x",b&255));asm.append("  ").append(in).append('\n');String source=in.getComment(CodeUnit.EOL_COMMENT);if(source!=null)asm.append("; ").append(source.replace("\n"," | ")).append('\n');
    for(Reference ref:in.getReferencesFrom()){JsonObject q=new JsonObject();q.addProperty("function",f.getEntryPoint().toString());q.addProperty("from",ref.getFromAddress().toString());q.addProperty("to",ref.getToAddress().toString());q.addProperty("type",ref.getReferenceType().toString());refs.add(q);}
    if(in.getFlowType().isComputed()){JsonObject q=new JsonObject();q.addProperty("function",f.getName());q.addProperty("address",in.getAddress().toString());q.addProperty("instruction",in.toString());q.addProperty("kind",in.getFlowType().toString());JsonArray flow=new JsonArray();for(Address a:in.getFlows())flow.add(a.toString());q.add("resolved_targets",flow);indirect.add(q);}
   }j.addProperty("instructions",count);functions.add(j);Files.writeString(out.resolve("raw/"+key+".asm"),asm.toString());if(++n%100==0)println("Exported "+n+" functions");
  }
  dec.dispose();Gson g=new GsonBuilder().setPrettyPrinting().create();Files.writeString(out.resolve("functions.json"),g.toJson(functions));Files.writeString(out.resolve("references.json"),g.toJson(refs));Files.writeString(out.resolve("indirect.json"),g.toJson(indirect));Files.writeString(out.resolve("switches.json"),g.toJson(tables));
 }
}
