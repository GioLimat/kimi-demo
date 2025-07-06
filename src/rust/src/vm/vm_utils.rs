use crate::vm::heap_manager::HeapManager;
use crate::vm::vm::VM;

impl VM {
    pub fn new(bytecode: Vec<u8>) -> Self {
        VM {
            stack: Vec::new(),
            bytecode,
            ip: 0,
            stack_ptr: 0,
            heap_manager: HeapManager::new(),
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
}



impl VM {
    pub fn read8(&mut self) -> u8 {
        self.ip += 1;
        let value = self.bytecode[self.ip as usize];
        self.ip += 1;
        value
    }
}

