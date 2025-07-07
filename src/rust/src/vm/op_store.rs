use crate::vm::vm::VM;

pub fn op_store(vm: &mut VM) {
    let value = vm.pop().unwrap_or(0);
    
    let frames_to_store = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let scopes_to_store = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32();

    let locals = vm.get_call_frame_scope_mut(frames_to_store as usize, scopes_to_store as usize);

    match locals {
        Some(scope) => {
            scope.insert(identifier_id as u64, value);
        },
        None => {
            println!("Error: Unable to find the specified scope for storing identifier with id: {}", identifier_id);
            return;
        }
    }
}