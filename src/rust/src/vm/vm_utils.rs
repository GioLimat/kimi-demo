use std::collections::HashMap;
use crate::vm::heap_manager::HeapManager;
use crate::vm::vm::{CallFrame, VM};

impl VM {
    pub fn new(bytecode: Vec<u8>) -> Self {
        let mut call_stack = Vec::<CallFrame>::new();
        
        call_stack.push(CallFrame::new());
        
        VM {
            stack: Vec::new(),
            bytecode,
            ip: 0,
            stack_ptr: 0,
            heap_manager: HeapManager::new(),
            call_stack
        }
    }

    pub fn push(&mut self, value: u64) {
        self.stack.push(value);
        self.stack_ptr += 1;
    }

    pub fn pop(&mut self) -> Option<u64> {
        if self.stack_ptr > 0 {
            self.stack_ptr -= 1;
            self.stack.pop()
        } else {
            None
        }
    }


    pub fn get_call_frame_scope_mut(
        &mut self,
        frame_depth: usize,
        scope_depth: usize,
    ) -> Option<&mut HashMap<u64, u64>> {
        let frame_index = self.call_stack.len().checked_sub(1 + frame_depth)?;
        let frame = self.call_stack.get_mut(frame_index)?;

        let scope_index = frame.locals.len().checked_sub(1 + scope_depth)?;
        frame.locals.get_mut(scope_index)
    }
}



impl VM {
    pub fn read_u8(&mut self) -> u8 {
        self.ip += 1;
        let value = self.bytecode[self.ip as usize];
        self.ip += 1;
        value
    }


    pub fn read_u16(&mut self) -> u16 {
        self.ip += 1;
        let mut bytes = [0u8; 2];
        for byte in 0..bytes.len() as usize {
            bytes[byte] = self.bytecode[self.ip as usize];
            self.ip += 1;
        }
        u16::from_le_bytes(bytes)
    }

    pub fn read_u32(&mut self) -> u32 {
        self.ip += 1;
        let mut bytes = [0u8; 4];
        for byte in 0..bytes.len() as usize {
            bytes[byte] = self.bytecode[self.ip as usize];
            self.ip += 1;
        }
        u32::from_le_bytes(bytes)
    }

    pub fn read_u64(&mut self) -> u64 {
        self.ip += 1;
        let mut bytes = [0u8; 8];
        for byte in 0..bytes.len() as usize {
            bytes[byte] = self.bytecode[self.ip as usize];
            self.ip += 1;
        }
        u64::from_le_bytes(bytes)
    }
    
    pub fn read_f32(&mut self) -> f32 {
        self.ip += 1;
        let mut bytes= [0u8; 4];
        for byte in 0..bytes.len() as usize {
            bytes[byte] = self.bytecode[self.ip as usize];
            self.ip += 1;
        }
        f32::from_le_bytes(bytes)
    }

    pub fn read_f64(&mut self) -> f64 {
        self.ip += 1;
        let mut bytes = [0u8; 8];
        for byte in 0..bytes.len() as usize {
            bytes[byte] = self.bytecode[self.ip as usize];
            self.ip += 1;
        }
        f64::from_le_bytes(bytes)
    }
}

