use crate::vm::vm::VM;




pub fn op_inc(vm: &mut VM) {
    let frames_to_load = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let scopes_to_load = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32() as u64;

    for frame in vm.call_stack.iter_mut().rev() {
        for scope in frame.locals.iter_mut().rev() {
            if let Some(val) = scope.get_mut(&identifier_id) {
                let v = *val;
                *val += 1;

                vm.push(v);
                return;
            }
        }
    }
}

pub fn op_dec(vm: &mut VM) {
    let frames_to_load = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let scopes_to_load = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32() as u64;

    for frame in vm.call_stack.iter_mut().rev() {
        for scope in frame.locals.iter_mut().rev() {
            if let Some(val) = scope.get_mut(&identifier_id) {
                let v = *val;
                *val -= 1;

                vm.push(v);
                return;
            }
        }
    }
}


pub fn op_post_inc(vm: &mut VM) {
}


pub fn op_post_dec(vm: &mut VM) {
}