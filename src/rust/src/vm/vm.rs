use std::collections::HashMap;
use crate::vm::heap_manager::HeapManager;
use crate::vm::op_add::op_add;
use crate::vm::op_const::op_const;
use crate::vm::op_load::op_load;
use crate::vm::op_println::op_println;
use crate::vm::op_store::op_store;

pub(crate) struct  CallFrame {
    pub(crate) return_ip: u64,
    pub(crate) locals: Vec<HashMap<u64, u64>>,
}


impl CallFrame {
    pub fn new() -> Self {
        let mut frame = CallFrame{
            return_ip: 0,
            locals: Vec::new(),
        };

        frame.locals.push(HashMap::new());
        
        frame
    }
}

pub struct VM {
    pub(crate) stack: Vec<u64>,
    pub(crate) bytecode: Vec<u8>,
    pub(crate) ip: u64,
    pub(crate) stack_ptr: u64,
    pub(crate) heap_manager: HeapManager,
    pub(crate) call_stack: Vec<CallFrame>,
    
}



impl VM {
    pub fn run(&mut self) {
        while self.ip < self.bytecode.len() as u64 {
            let opcode = self.bytecode[self.ip as usize];
            self.ip += 1;

            match opcode {
                0x00 => { // op_halt
                    println!("\n\n------------------Halting VM------------------");
                    break;
                }
                0x01 => op_const(self), // op_const
                0x02 => op_load(self), // op_load
                0x03 => op_store(self), // op_store
                0x04 => op_println(self), // op_println
                0x0E => op_add(self), // op_add
                _ => {
                    println!("Unknown opcode: {}", opcode);
                    break;
                }
            }
        }
    }
}