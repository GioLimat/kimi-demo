use crate::vm::vm::VM;

pub fn op_const(vm: &mut VM) {
    let op_type = vm.bytecode.get(vm.ip as usize).unwrap();

    
    match op_type {
        0x01 | 0x09 => {
            let value = vm.read_u32() as u64;
            vm.push(value);
        }
        0x02 => {
            let value = vm.read_u64();
            vm.push(value);
        }
        0x03 => {
            let value = vm.read_f32();
            vm.push(value.to_bits().into());
        }
        0x04 => {
            let value = vm.read_f64();
            vm.push(value.to_bits());
        }

        0x05 | 0x07 => {
            let value = vm.read_u8() as u64;
            vm.push(value);
        }

        0x08 => {
            let value = vm.read_u16() as u64;
            vm.push(value);
        }

        _ => {
            println!("Unknown constant type: {}", op_type);
        }
    }
}