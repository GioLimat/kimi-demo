use crate::vm::vm::VM;

pub fn op_const(vm: &mut VM) {
    let op_type = vm.bytecode.get(vm.ip as usize).unwrap();

    
    match op_type { 
        0x07 => {
            let value = vm.read8() as u64;
            vm.push(value);
        }
        _ => {
            println!("Unknown constant type: {}", op_type);
        }
    }
}