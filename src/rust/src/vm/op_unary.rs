use crate::vm::vm::VM;

pub fn op_neg(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw = vm.pop().expect("Stack underflow on op_neg");

    match op_type {
        0x03 => {
            let value = -f32::from_bits(raw as u32);
            vm.push(value.to_bits() as u64);
        }
        0x04 => {
            let value = -f64::from_bits(raw);
            vm.push(value.to_bits());
        }
        _ => {
            let value = -(raw as i64);
            vm.push(value as u64);
        }
    }
}


pub fn op_not(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw = vm.pop().expect("Stack underflow on op_not");

    match op_type {
        0x03 => {
            let value = f32::from_bits(raw as u32) == 0.0;
            vm.push(value as u64);
        }
        0x04 => {
            let value = f64::from_bits(raw) == 0.0;
            vm.push(value as u64);
        }
        _ => {
            vm.push((raw == 0) as u64);
        }
    }
}


pub fn op_bit_not(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw = vm.pop().expect("Stack underflow on op_bit_not");

    vm.push(!raw);
}