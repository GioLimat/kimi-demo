use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;

fn dispatch_numeric<F32, F64, SI, UI>(
    vm: &mut VM,
    f32_op: F32,
    f64_op: F64,
    si_op: SI,
    ui_op: UI,
) where
    F32: Fn(f32, f32) -> f32,
    F64: Fn(f64, f64) -> f64,
    SI: Fn(i64, i64) -> i64,
    UI: Fn(u64, u64) -> u64,
{
    let t = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    let r = match t {
        0x03 => f32_op(f32::from_bits(a as u32), f32::from_bits(b as u32)).to_bits() as u64,
        0x04 => f64_op(f64::from_bits(a), f64::from_bits(b)).to_bits(),
        0x01 | 0x02 | 0x07 | 0x08 => si_op(a as i64, b as i64) as u64,
        0x09 | 0x0C | 0x0D | 0x0E | 0x0F | 0x05 => ui_op(a, b),
        _    => si_op(a as i64, b as i64) as u64,
    };
    vm.push(r);
}

fn dispatch_bool<F32, F64, SI, UI>(
    vm: &mut VM,
    f32_op: F32,
    f64_op: F64,
    si_op: SI,
    ui_op: UI,
) where
    F32: Fn(f32, f32) -> bool,
    F64: Fn(f64, f64) -> bool,
    SI: Fn(i64, i64) -> bool,
    UI: Fn(u64, u64) -> bool,
{
    dispatch_numeric(
        vm,
        |a, b| (f32_op(a, b) as u32) as f32,
        |a, b| (f64_op(a, b) as u64) as f64,
        |a, b| si_op(a, b) as i64,
        |a, b| ui_op(a, b) as u64,
    )
}

pub fn op_add(vm: &mut VM) {
    if vm.bytecode[vm.ip as usize] == 0x0A {
        vm.ip += 1;
        let b = vm.pop().unwrap();
        let a = vm.pop().unwrap();
        let sa = vm.heap_manager.get(a).unwrap().as_string().unwrap();
        let sb = vm.heap_manager.get(b).unwrap().as_string().unwrap();
        let r = vm.heap_manager.alloc(HeapValue::String(format!("{}{}", sa, sb)));
        vm.push(r);
    } else {
        dispatch_numeric(vm, |a, b| a + b, |a, b| a + b, |a, b| a + b, |a, b| a + b);
    }
}

pub fn op_sub(vm: &mut VM) {
    dispatch_numeric(vm, |a, b| a - b, |a, b| a - b, |a, b| a - b, |a, b| a - b);
}

pub fn op_mul(vm: &mut VM) {
    dispatch_numeric(vm, |a, b| a * b, |a, b| a * b, |a, b| a * b, |a, b| a * b);
}

pub fn op_div(vm: &mut VM) {
    dispatch_numeric(vm, |a, b| a / b, |a, b| a / b, |a, b| a / b, |a, b| a / b);
}

pub fn op_mod(vm: &mut VM) {
    dispatch_numeric(vm, |a, b| a % b, |a, b| a % b, |a, b| a % b, |a, b| a % b);
}

pub fn op_equal(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a == b, |a, b| a == b, |a, b| a == b, |a, b| a == b);
}

pub fn op_not_equal(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a != b, |a, b| a != b, |a, b| a != b, |a, b| a != b);
}

pub fn op_greater(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a > b, |a, b| a > b, |a, b| a > b, |a, b| a > b);
}

pub fn op_less(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a < b, |a, b| a < b, |a, b| a < b, |a, b| a < b);
}

pub fn op_greater_equal(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a >= b, |a, b| a >= b, |a, b| a >= b, |a, b| a >= b);
}

pub fn op_less_equal(vm: &mut VM) {
    dispatch_bool(vm, |a, b| a <= b, |a, b| a <= b, |a, b| a <= b, |a, b| a <= b);
}

pub fn op_and(vm: &mut VM) {
    dispatch_bool(
        vm,
        |a, b| a != 0.0 && b != 0.0,
        |a, b| a != 0.0 && b != 0.0,
        |a, b| a != 0 && b != 0,
        |a, b| a != 0 && b != 0,
    );
}

pub fn op_or(vm: &mut VM) {
    dispatch_bool(
        vm,
        |a, b| a != 0.0 || b != 0.0,
        |a, b| a != 0.0 || b != 0.0,
        |a, b| a != 0 || b != 0,
        |a, b| a != 0 || b != 0,
    );
}

pub fn op_bit_and(vm: &mut VM) {
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    vm.push(a & b);
}

pub fn op_bit_or(vm: &mut VM) {
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    vm.push(a | b);
}

pub fn op_bit_xor(vm: &mut VM) {
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    vm.push(a ^ b);
}

pub fn op_shift_left(vm: &mut VM) {
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    vm.push(a << b);
}

pub fn op_shift_right(vm: &mut VM) {
    vm.ip += 1;
    let b = vm.pop().unwrap();
    let a = vm.pop().unwrap();
    vm.push(a >> b);
}
