use crate::vm::heap_manager::HeapValue;
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

pub fn op_store_array_element(vm: &mut VM) {
    let value = vm.pop().expect("Stack underflow: expected value");
    let index = vm.pop().expect("Stack underflow: expected index");
    let array_id = vm.pop().expect("Stack underflow: expected array reference");

    let heap_val = vm
        .heap_manager
        .get_mut(array_id)
        .expect("Invalid heap ID for array store");

    if let HeapValue::Array { elements, .. } = heap_val {
        let i = index as usize;
        if i >= elements.len() {
            panic!("Array index out of bounds: {}", i);
        }
        elements[i] = value;
    } else {
        panic!("Tried to store into non-array heap object: {}", array_id);
    }
}