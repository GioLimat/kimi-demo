use crate::vm::heap_manager::HeapManager;
use crate::vm::op_add::op_add;
use crate::vm::op_const::op_const;

pub struct VM {
    pub(crate) stack: Vec<u64>,
    pub(crate) bytecode: Vec<u8>,
    pub(crate) ip: u64,
    pub(crate) stack_ptr: u64,
    pub(crate) heap_manager: HeapManager
}



impl VM {
    pub fn run(&mut self) {
        while self.ip < self.bytecode.len() as u64 {
            let opcode = self.bytecode[self.ip as usize];
            self.ip += 1;

            match opcode {
                0x00 => { // op_halt
                    println!("Halting VM.");
                    break;
                }
                0x01 => op_const(self), // op_const
                0x0E => op_add(self), // op_add
                _ => {
                    println!("Unknown opcode: {}", opcode);
                    break;
                }
            }
        }
    }
}