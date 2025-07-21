use std::process::id;
use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;

pub fn op_store(vm: &mut VM) {
    let value = vm.pop().unwrap_or(0);
    
    let frames_to_store = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let scopes_to_store = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32();

    let mut found = false;

    for frame in vm.call_stack.iter_mut().rev() {
       for scope in frame.locals.iter_mut().rev() {
           let entry = scope.get_mut(&(identifier_id as u64));
           if let Some(k) = entry {
               *k = value;
               found = true;
               break;
           }
       }
    }

    if !found {
        vm.call_stack.last_mut().unwrap().locals.last_mut().unwrap().insert(identifier_id as u64, value);
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