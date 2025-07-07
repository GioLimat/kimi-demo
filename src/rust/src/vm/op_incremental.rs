use crate::vm::vm::VM;

fn read_inc(vm: &mut VM) -> (u32, Option<&mut std::collections::HashMap<u64, u64>>) {
    let frames_to_store = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let scopes_to_store = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32();

    let locals = vm.get_call_frame_scope_mut(frames_to_store as usize, scopes_to_store as usize);

    (identifier_id, locals)
}

fn read_value(vm: &mut VM, inc: i64) -> u64 {
    let (identifier_id, locals) = read_inc(vm);

    let value = {
        if let Some(scope) = locals {
            let v = *scope.get(&(identifier_id as u64)).expect("Error: identifier not found") as i64 + inc;
            scope.insert(identifier_id as u64, v as u64);
            v
        } else {
            println!("Error: Unable to find the specified scope for identifier with id: {}", identifier_id);
            return 0;
        }
    };
    value as u64
}

pub fn op_inc(vm: &mut VM) {
    let value = read_value(vm, 1);
    
    vm.push(value);
}

pub fn op_dec(vm: &mut VM) {
    let value = read_value(vm, -1);

    vm.push(value);
}


pub fn op_post_inc(vm: &mut VM) {
    let value = read_value(vm, 1);

    vm.push(value - 1);
}


pub fn op_post_dec(vm: &mut VM) {
    let value = read_value(vm, -1);

    vm.push(value + 1);
}