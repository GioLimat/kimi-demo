use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;

pub fn op_add(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_add");
    let raw_a = vm.pop().expect("Stack underflow on op_add");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) + f32::from_bits(raw_b as u32);
            vm.push(value.to_bits() as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) + f64::from_bits(raw_b);
            vm.push(value.to_bits());
        }
        0x0A => {
            let string_a = vm.heap_manager.get(raw_a).expect("Fail to unpack string concatenation").as_string().unwrap();
            let string_b = vm.heap_manager.get(raw_b).expect("Fail to unpack string concatenation").as_string().unwrap();
            
            let new_string = format!("{}{}", string_a, string_b);
            let heap_value = vm.heap_manager.alloc(HeapValue::String(new_string));
            
            vm.push(heap_value);
        }
        _ => {
            vm.push(raw_a + raw_b);

        }
    }
}


pub fn op_sub(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_sub");
    let raw_a = vm.pop().expect("Stack underflow on op_sub");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) - f32::from_bits(raw_b as u32);
            vm.push(value.to_bits() as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) - f64::from_bits(raw_b);
            vm.push(value.to_bits());
        }

        _ => {
            vm.push(raw_a - raw_b);

        }
    }
}

pub fn op_mul(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_mul");
    let raw_a = vm.pop().expect("Stack underflow on op_mul");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) * f32::from_bits(raw_b as u32);
            vm.push(value.to_bits() as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) * f64::from_bits(raw_b);
            vm.push(value.to_bits());
        }

        _ => {
            vm.push(raw_a * raw_b);

        }
    }
}


pub fn op_div(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_div");
    let raw_a = vm.pop().expect("Stack underflow on op_div");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) / f32::from_bits(raw_b as u32);
            vm.push(value.to_bits() as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) / f64::from_bits(raw_b);
            vm.push(value.to_bits());
        }

        _ => {
            vm.push(raw_a / raw_b);

        }
    }
}

pub fn op_mod(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_mod");
    let raw_a = vm.pop().expect("Stack underflow on op_mod");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) % f32::from_bits(raw_b as u32);
            vm.push(value.to_bits() as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) % f64::from_bits(raw_b);
            vm.push(value.to_bits());
        }

        _ => {
            vm.push(raw_a % raw_b);

        }
    }
}


pub fn op_equal(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_equal");
    let raw_a = vm.pop().expect("Stack underflow on op_equal");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) == f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) == f64::from_bits(raw_b);
            vm.push(value as u64);
        }

        _ => {
            vm.push((raw_a == raw_b) as u64);

        }
    }
}



pub fn op_greater(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_greater");
    let raw_a = vm.pop().expect("Stack underflow on op_greater");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) > f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) > f64::from_bits(raw_b);
            vm.push(value as u64);
        }

        _ => {
            vm.push((raw_a > raw_b) as u64);

        }
    }
}




pub fn op_less(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_less");
    let raw_a = vm.pop().expect("Stack underflow on op_less");

    match op_type  {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) < f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        // f64
        0x04 => {
            let value = f64::from_bits(raw_a) < f64::from_bits(raw_b);
            vm.push(value as u64);
        }

        _ => {
            vm.push((raw_a < raw_b) as u64);

        }
    }
}


pub fn op_greater_equal(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_greater_equal");
    let raw_a = vm.pop().expect("Stack underflow on op_greater_equal");

    match op_type {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) >= f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        0x04 => {
            let value = f64::from_bits(raw_a) >= f64::from_bits(raw_b);
            vm.push(value as u64);
        }
        _ => {
            vm.push((raw_a >= raw_b) as u64);
        }
    }
}



pub fn op_less_equal(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_less_equal");
    let raw_a = vm.pop().expect("Stack underflow on op_less_equal");

    match op_type {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) <= f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        0x04 => {
            let value = f64::from_bits(raw_a) <= f64::from_bits(raw_b);
            vm.push(value as u64);
        }
        _ => {
            vm.push((raw_a <= raw_b) as u64);
        }
    }
}



pub fn op_not_equal(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_not_equal");
    let raw_a = vm.pop().expect("Stack underflow on op_not_equal");

    match op_type {
        0x03 => {
            let value = f32::from_bits(raw_a as u32) != f32::from_bits(raw_b as u32);
            vm.push(value as u64);
        }
        0x04 => {
            let value = f64::from_bits(raw_a) != f64::from_bits(raw_b);
            vm.push(value as u64);
        }
        _ => {
            vm.push((raw_a != raw_b) as u64);
        }
    }
}



pub fn op_and(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_and");
    let raw_a = vm.pop().expect("Stack underflow on op_and");

    match op_type {
        0x03 => {
            let value = (f32::from_bits(raw_a as u32) != 0.0) && (f32::from_bits(raw_b as u32) != 0.0);
            vm.push(value as u64);
        }
        0x04 => {
            let value = (f64::from_bits(raw_a) != 0.0) && (f64::from_bits(raw_b) != 0.0);
            vm.push(value as u64);
        }
        _ => {
            vm.push(((raw_a != 0) && (raw_b != 0)) as u64);
        }
    }
}


pub fn op_or(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw_b = vm.pop().expect("Stack underflow on op_or");
    let raw_a = vm.pop().expect("Stack underflow on op_or");

    match op_type {
        0x03 => {
            let value = (f32::from_bits(raw_a as u32) != 0.0) || (f32::from_bits(raw_b as u32) != 0.0);
            vm.push(value as u64);
        }
        0x04 => {
            let value = (f64::from_bits(raw_a) != 0.0) || (f64::from_bits(raw_b) != 0.0);
            vm.push(value as u64);
        }
        _ => {
            vm.push(((raw_a != 0) || (raw_b != 0)) as u64);
        }
    }
}

pub fn op_bit_and(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let b = vm.pop().expect("Stack underflow on op_bit_and");
    let a = vm.pop().expect("Stack underflow on op_bit_and");

    vm.push(a & b);
}

pub fn op_bit_or(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let b = vm.pop().expect("Stack underflow on op_bit_or");
    let a = vm.pop().expect("Stack underflow on op_bit_or");

    vm.push(a | b);
}

pub fn op_bit_xor(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let b = vm.pop().expect("Stack underflow on op_bit_xor");
    let a = vm.pop().expect("Stack underflow on op_bit_xor");

    vm.push(a ^ b);
}

pub fn op_shift_left(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let b = vm.pop().expect("Stack underflow on op_shift_left");
    let a = vm.pop().expect("Stack underflow on op_shift_left");

    vm.push(a << b);
}

pub fn op_shift_right(vm: &mut VM) {
    let _op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let b = vm.pop().expect("Stack underflow on op_shift_right");
    let a = vm.pop().expect("Stack underflow on op_shift_right");

    vm.push(a >> b);
}