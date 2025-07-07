use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;

pub fn op_alloc(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    let len = vm.read_u32();
    vm.ip -= 1; // Adjust IP after reading the length
    let bytes = vm.read_u32();

    let mut value_bytes = Vec::<u8>::new();

    for byte in vm.ip..vm.ip + bytes as u64 {
        value_bytes.push(vm.bytecode[byte as usize]);
    }
    
    vm.ip += bytes as u64; // Move IP forward after reading the value bytes
    
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