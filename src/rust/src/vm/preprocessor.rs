use std::slice;
use crate::vm::vm::VM;

#[unsafe(no_mangle)]
pub extern "C" fn run_vm(data: *const u8, len: usize){
    let slice = unsafe {
        assert!(!data.is_null());
        slice::from_raw_parts(data, len)
    };

    let vec: Vec<u8> = slice.to_vec();

    println!("Bytecode Mapper: {:?}", vec);
    println!("Invocation successful!");
    
    
    VM::new(vec).run();
}