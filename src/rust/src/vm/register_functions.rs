use std::collections::HashMap;
use crate::vm::vm::{VM, FunctionInfo};

impl VM {
    fn decode_type(&self, t: u8) -> u64 {
        match t {
            0x01 => 4,
            0x02 => 8,
            0x03 => 4,
            0x04 => 8,
            0x05 => 1,
            0x06 => 8,
            0x07 => 1,
            0x08 => 2,
            0x09 => 4,
            0x0a => 8,
            0x0B => 8,
            0x0C => 8,
            0x0D => 16,
            0x0E => 32,
            0x0F => 64,
            _ => 0,
        }
    }

    fn get_instruction_length(&self, op: u8) -> u64 {
        match op {
            0x01 => {
                2 + self.decode_type(self.bytecode[(self.ip + 1) as usize])
            },
            0x02 | 0x1D | 0x1E | 0x1F | 0x20 | 0x03 => 7,
            0x1A | 0x1B | 0x05 | 0x16 | 0x06 => 5,
            0x07 | 0x08 | 0x19 | 0x33 => 1,
            0x36 => 3,
            0x04 | 0x0E | 0x0F | 0x10 | 0x11 | 0x12 |
            0x13 | 0x14 | 0x15 | 0x18 | 0x1C | 0x21 |
            0x22 | 0x23 | 0x24 | 0x25 | 0x26 | 0x27 |
            0x28 | 0x29 | 0x2A | 0x2B | 0x2C | 0x31 | 0x34 | 0x35  => 2,
            0x2E => {
                let bytes = &self.bytecode[(self.ip  + 5) as usize..(self.ip + 9) as usize];
                5 + u32::from_le_bytes(bytes.try_into().unwrap())  as u64
            },
            0x32 => {
                6
            },
            0x00 => 1,
            _ => 0,
        }
    }

    pub fn preprocess_functions(&mut self) {
        while self.ip < self.bytecode.len() as u64 {
            if self.bytecode[self.ip as usize] == 0x05 {
                self.ip += 5;
                let mut return_ip = self.ip;
                self.register_function(&mut return_ip);
                self.ip = return_ip;
            } else {
                let op = self.bytecode[self.ip as usize];
                self.ip += self.get_instruction_length(op);
            }
        }
        self.ip = 0;
    }

    fn register_function(&mut self, return_pos: &mut u64) {
        let start_ip = self.ip;
        let mut end_ip = 0;
        let mut args = 0;
        let params_ids = Vec::<u32>::new();
        self.functions.push(FunctionInfo {
            param_count: 0,
            start_ip,
            end_ip: 0,
            params_ids
        });
        let idx = self.functions.len() - 1;
        while self.ip < self.bytecode.len() as u64 {
            let op = self.bytecode[self.ip as usize];
            if op == 0x05 {
                self.ip += 5;
                let mut nested_end = 0;
                self.register_function(&mut nested_end);
                self.ip = nested_end + 1;
                continue;
            }
            if op == 0x06 {
                args += 1;

                let id = self.read_u32();
              
                self.functions.get_mut(idx).unwrap().params_ids.push(id);
                self.ip -= 5;
            } else if op == 0x19 {
                end_ip = self.ip;
                break;
            }
            self.ip += self.get_instruction_length(op);
        }
        if let Some(f) = self.functions.get_mut(idx) {
            f.end_ip = end_ip;
            f.param_count = args;
        }
        *return_pos = end_ip;
    }
}