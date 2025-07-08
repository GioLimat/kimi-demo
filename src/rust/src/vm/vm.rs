use std::collections::HashMap;
use crate::vm::heap_manager::HeapManager;
use crate::vm::op_binary::*;
use crate::vm::op_const::op_const;
use crate::vm::op_function::*;
use crate::vm::op_incremental::*;
use crate::vm::op_index_access::op_index_access;
use crate::vm::op_load::op_load;
use crate::vm::op_println::op_println;
use crate::vm::op_scope_points::*;
use crate::vm::op_store::op_store;
use crate::vm::op_unary::*;
use crate::vm::op_jmp_if::*;
use crate::vm::op_memory::*;

pub(crate) struct  CallFrame {
    pub(crate) return_ip: u64,
    pub(crate) locals: Vec<HashMap<u64, u64>>,
}

#[derive(Debug)]
pub(crate) struct FunctionInfo {
    pub(crate) param_count: u32,
    pub(crate) start_ip: u64,
    pub(crate) end_ip: u64,
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
    pub(crate) functions: Vec<FunctionInfo>,
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
                0x05 => op_function(self), // op_function
                0x06 => op_fn_param(self), // op_fn_param
                0x07 => op_end_block(self), // op_end_block
                0x08 => op_init_block(self), // op_init_block
                0x0E => op_add(self), // op_add
                0x0F => op_sub(self), // op_sub
                0x10 => op_mul(self), // op_mul
                0x11 => op_div(self), // op_div
                0x12 => op_mod(self), // op_mod
                0x13 => op_greater(self), // op_greater
                0x14 => op_less(self), // op_less
                0x15 => op_equal(self), // op_equal
                0x16 => op_call(self), // op_call
                0x18 => op_return(self), // op_return
                0x19 => op_end_fn_block(self), // op_end_fn_block
                0x1A => op_if_false(self), // op_if_false
                0x1B => op_jmp(self), // op_jmp
                0x1C => op_neg(self), // op_neg
                0x1D => op_inc(self), // op_inc
                0x1E => op_dec(self), // op_dec
                0x1F => op_post_inc(self), // op_post_inc
                0x20 => op_post_dec(self), // op_post_dec
                0x21 => op_greater_equal(self), // op_greater_equal
                0x22 => op_less_equal(self), // op_less_equal
                0x23 => op_not_equal(self), // op_not_equal
                0x24 => op_and(self), // op_and
                0x25 => op_or(self), // op_or
                0x26 => op_not(self), // op_not
                0x27 => op_bit_and(self), // op_bit_and
                0x28 => op_bit_or(self), // op_bit_or
                0x29 => op_bit_xor(self), // op_bit_xor
                0x2A => op_shift_left(self), // op_shift_left
                0x2B => op_shift_right(self), // op_shift_right
                0x2C => op_bit_not(self), // op_bit_not
                0x2E => op_alloc(self), // op_alloc
                0x31 => op_index_access(self), // op_index_access
                0x32 => op_alloc_array(self), // op_alloc_array
                _ => {
                    println!("Unknown opcode: 0x{:02X}", opcode);
                    break;
                }
            }
        }
    }
}