use crate::vm::vm::VM;

pub fn op_load(vm: &mut VM) {
    let frames_to_load = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let scopes_to_load = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32() as u64;


    for frame in vm.call_stack.iter().rev() {
        for scope in frame.locals.iter().rev() {
            if let Some(&val) = scope.get(&identifier_id) {
                vm.push(val);
                return;
            }
        }
    }
}
