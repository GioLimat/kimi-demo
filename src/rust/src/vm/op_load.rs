use crate::vm::vm::VM;

pub fn op_load(vm: &mut VM) {
    let frames_to_load = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let scopes_to_load = vm.bytecode[vm.ip as usize];

    let identifier_id = vm.read_u32() as u64;

    let value_to_push = {
        let locals = vm
            .get_call_frame_scope_mut(frames_to_load as usize, scopes_to_load as usize)
            .expect(&format!(
                "Error: frame {} or scope {} not found",
                frames_to_load, scopes_to_load
            ));
        
        *locals.get(&identifier_id).expect("Error: identifier not found")
    };
    
    vm.push(value_to_push);
}
