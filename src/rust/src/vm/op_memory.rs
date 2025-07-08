use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;


fn read_allocation(vm: &mut VM) -> (u8, Vec<u8>, u32) {
    let op_type = vm.bytecode[vm.ip as usize];
    let len = vm.read_u32();
    vm.ip -= 1; // Adjust IP after reading the length
    let bytes = vm.read_u32();

    let mut value_bytes = Vec::<u8>::new();

    for byte in vm.ip..vm.ip + bytes as u64 {
        value_bytes.push(vm.bytecode[byte as usize]);
    }

    vm.ip += bytes as u64; // Move IP forward after reading the value bytes
    
    println!("{len  } bytes read for allocation type {op_type}");
    
    (op_type, value_bytes, len)
}

pub fn op_alloc(vm: &mut VM) {
    let (op_type, value_bytes, _) = read_allocation(vm);
    
    match op_type {
        0x0A => {
            let string_value = String::from_utf8(value_bytes).unwrap();

            let heap_value = HeapValue::String(string_value.clone());

            let id = vm.heap_manager.alloc(heap_value);

            vm.push(id);
        }
        _ => {
            println!("Unknown allocation type: {op_type}");
        }
    }
}


pub fn op_alloc_array(vm: &mut VM) {
    vm.ip -= 1;
    let elem_type = vm.read_u8();              
    let elem_type_id = None;
    vm.ip -= 1;
    let len = vm.read_u32() as usize;           
    
    let mut elements = Vec::with_capacity(len);
    for _ in 0..len {
        elements.push(vm.pop().expect("stack underflow"));
    }
    elements.reverse(); 

    let id = vm.heap_manager.alloc(HeapValue::Array {
        elem_type,
        elem_type_id,
        elements,
    });
    vm.push(id);
}