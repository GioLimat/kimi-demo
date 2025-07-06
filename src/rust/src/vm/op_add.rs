use crate::vm::vm::VM;

pub fn op_add(vm: &mut VM) {
    let op_type = vm.bytecode.get(vm.ip as usize).unwrap();
    vm.ip += 1;

    match op_type  {
        _ => {
            let b = vm.pop().expect("Stack underflow on op_add");
            let a = vm.pop().expect("Stack underflow on op_add");
            vm.push(a + b);

        }
    }
}