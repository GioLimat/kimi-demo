use crate::vm::vm::VM;

pub fn op_jmp(vm: &mut VM){
    vm.ip -= 1; // Adjust for the offset read below
    let offset = vm.read_i32() as i64;
    vm.ip = (vm.ip as i64 + offset) as u64;
}

pub fn op_if_false(vm: &mut VM) {
    vm.ip -= 1; // Adjust for the offset read below
    let offset = vm.read_u32() as u64;
    let value = vm.pop().unwrap();

    if value == 0 {
        vm.ip += offset;
    }
}