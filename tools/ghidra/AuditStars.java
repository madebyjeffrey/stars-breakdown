// @category Stars
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;
public class AuditStars extends GhidraScript {
 public void run()throws Exception{
  Path out=Path.of(getScriptArgs()[0]);JsonArray units=new JsonArray(),bad=new JsonArray(),symbols=new JsonArray();
  for(MemoryBlock b:currentProgram.getMemory().getBlocks())if(b.getName().matches("(Code|Data)[0-9]+")){
   Address a=b.getStart();while(a.compareTo(b.getEnd())<=0){CodeUnit u=currentProgram.getListing().getCodeUnitAt(a);int len=u==null?1:u.getLength();String kind=u instanceof Instruction?"instruction":u instanceof Data&&((Data)u).isDefined()?"data":"undefined";JsonObject r=new JsonObject();r.addProperty("segment",Integer.parseInt(b.getName().replaceAll("[^0-9]","")));r.addProperty("start",((SegmentedAddress)a).getSegmentOffset());r.addProperty("size",len);r.addProperty("kind",kind);if(!kind.equals("undefined"))r.addProperty("text",u.toString());units.add(r);a=a.add(len);}
  }
  SymbolIterator it=currentProgram.getSymbolTable().getAllSymbols(true);while(it.hasNext()){Symbol s=it.next();if(!s.getAddress().isMemoryAddress())continue;JsonObject r=new JsonObject();r.addProperty("address",s.getAddress().toString());r.addProperty("name",s.getName(true));r.addProperty("type",s.getSymbolType().toString());symbols.add(r);}
  InstructionIterator ins=currentProgram.getListing().getInstructions(true);while(ins.hasNext()){Instruction in=ins.next();for(Address a:in.getFlows()){MemoryBlock b=currentProgram.getMemory().getBlock(a);if(b==null||(!b.isArtificial()&&getInstructionAt(a)==null)){JsonObject r=new JsonObject();r.addProperty("from",in.getAddress().toString());r.addProperty("to",a.toString());r.addProperty("instruction",in.toString());bad.add(r);}}}
  Gson g=new GsonBuilder().create();Files.writeString(out.resolve("units.json"),g.toJson(units));Files.writeString(out.resolve("symbols.json"),g.toJson(symbols));Files.writeString(out.resolve("invalid-flows.json"),new GsonBuilder().setPrettyPrinting().create().toJson(bad));
  Files.writeString(out.resolve("analysis-settings.json"),new GsonBuilder().setPrettyPrinting().create().toJson(new TreeMap<>(getCurrentAnalysisOptionsAndValues(currentProgram))));
 }
}
