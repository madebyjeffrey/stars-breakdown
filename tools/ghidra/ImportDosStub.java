// Analyze the separately mapped real-mode DOS rejection stub.
// @category Stars
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.math.BigInteger;
public class ImportDosStub extends GhidraScript {
 public void run() throws Exception {
  Address start=toAddr("1000:0000"), helper=toAddr("1000:0056");
  currentProgram.getProgramContext().setValue(currentProgram.getRegister("CS"),start,start.add(0x61),BigInteger.valueOf(0x1000));
  createData(start.add(3),new ArrayDataType(ByteDataType.dataType,0x53,1));
  createLabel(start.add(3),"requires_windows_message",true);
  new DisassembleCommand(helper,new AddressSet(helper,helper.add(11)),true).applyTo(currentProgram,monitor);
  Function print=currentProgram.getFunctionManager().createFunction("dos_print_and_exit",helper,new AddressSet(helper,helper.add(11)),SourceType.IMPORTED);
  getInstructionAt(helper.add(10)).setFlowOverride(FlowOverride.CALL_RETURN);
  print.setReturnType(VoidDataType.dataType,SourceType.IMPORTED);print.setCallingConvention("__cdecl16near");print.setStackPurgeSize(0);
  print.setNoReturn(true);print.setComment("File 0x256: POP DX takes CALL's return address (message at file 0x203). DS=CS; INT 21h/AH=09 prints the $-terminated message; INT 21h/AX=4c01 exits with status 1. Register/interrupt pseudocode is raw; these semantics come from the original instructions.");
  new DisassembleCommand(start,new AddressSet(start,start.add(2)),true).applyTo(currentProgram,monitor);
  Function entry=currentProgram.getFunctionManager().createFunction("dos_entry",start,new AddressSet(start,start.add(2)),SourceType.IMPORTED);
  entry.setReturnType(VoidDataType.dataType,SourceType.IMPORTED);entry.setCallingConvention("__cdecl16near");entry.setStackPurgeSize(0);
  entry.setNoReturn(true);entry.setComment("MZ entry at file 0x200. This near CALL never returns; following bytes are message/padding, not instructions.");
 }
}
