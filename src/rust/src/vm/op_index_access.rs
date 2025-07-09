use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;


pub fn op_index_access(vm: &mut VM) {
    vm.ip -= 1; // Adjust for the opcode read below
    let op_type = vm.read_u8();

    let index = vm.pop().expect("Stack underflow on op_index_access") as usize;
    let operand = vm.pop().expect("Stack underflow on op_index_access");

    match op_type {
        0x0A => {
            let string = vm.heap_manager.get(operand).unwrap().as_string().unwrap();

            if (index < string.len()) {
                let char_at_index = string.chars().nth(index).unwrap();
                let heap_value = vm.heap_manager.alloc(HeapValue::String(char_at_index.to_string()));
                vm.push(heap_value);
            } else {
                println!("Error: Index out of bounds for string access");
            }
        }
        0x0B => {
            let (elem_type, elem_type_id, elements) = vm.heap_manager.get(operand).unwrap().as_array().unwrap();

            if index < elements.len() {
                let value = elements[index];
                vm.push(value);
            } else {
                println!("Error: Index out of bounds for array access");
            }
        }
        _ => {
            println!("Error: Unsupported index access operation type: {}", op_type);
        }
    }
}