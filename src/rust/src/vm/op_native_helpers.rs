use crate::vm::vm::VM;

pub fn op_insert_(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let heap_ptr = vm.stack.pop().expect("Stack underflow");
    let index = vm.stack.pop().expect("Stack underflow");
    let value = vm.stack.pop().expect("Stack underflow");

    let item = vm.heap_manager.get_mut(heap_ptr);


    match op_type {
        0x0B => {
            let  (_, _, elements) = item.unwrap().as_array_mut().unwrap();
            let len = elements.len() as u64;

            if index == len { elements.push(value); }
            else if index > len {
                panic!("Index out of bounds for array insert operation");
            }
            else {
                elements.insert(index as usize, value);
            }

        }
        _ => panic!("Unknown insert operation type: {}", op_type),
    }
}



pub fn op_remove_(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let heap_ptr = vm.stack.pop().expect("Stack underflow");
    let index = vm.stack.pop().expect("Stack underflow");

    let item = vm.heap_manager.get_mut(heap_ptr);


    match op_type {
        0x0B => {
            let  (_, _, elements) = item.unwrap().as_array_mut().unwrap();
            let len = elements.len() as u64;

            if index == len - 1 { elements.pop(); }
            else if index > len {
                panic!("Index out of bounds for array insert operation");
            }
            else {
                elements.remove(index as usize);
            }

        }
        _ => panic!("Unknown insert operation type: {}", op_type),
    }
}