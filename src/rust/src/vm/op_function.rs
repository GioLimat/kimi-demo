use crate::vm::vm::{CallFrame, VM};

pub fn op_function(vm: &mut VM) {
    vm.ip -= 1; // Adjust IP to point to read the function ID
    let id = vm.read_u32();
    
    let function_info = &vm.functions[id as usize];
    
    vm.ip = function_info.end_ip;
}

pub fn op_return(vm: &mut VM) {
    vm.ip -= 1; // Adjust IP to point to read the function ID
    let _op_type = vm.read_u8();
    
    let frame = vm.call_stack.last_mut().expect("Call stack is empty");

    vm.ip = frame.return_ip;

    vm.call_stack.pop();
}


pub fn op_call(vm: &mut VM) {
    vm.ip -= 1; // Adjust IP to point to read the function ID
    let id = vm.read_u32();

    let function_info = vm.functions[id as usize].clone();

    let mut frame = CallFrame::new();

    frame.return_ip = vm.ip;

    vm.ip = function_info.start_ip;

    for param_id in function_info.params_ids {
        let value = vm.pop().unwrap();
        frame.locals[0].insert(param_id as u64, value);
    }

    vm.call_stack.push(frame);
}