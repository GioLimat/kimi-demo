use std::collections::HashMap;
use crate::vm::vm::VM;


pub fn op_init_block(vm: &mut VM) {
    vm.call_stack
        .last_mut()              
        .expect("no call frame")
        .locals
        .push(HashMap::new());
}

pub fn op_end_block(vm: &mut VM) {
    vm.call_stack
        .last_mut()
        .expect("no call frame")
        .locals.pop();
}


pub fn op_end_fn_block(_vm: &mut VM){
    return;
}


pub fn op_fn_param(vm: &mut VM){
    vm.ip += 4;
}