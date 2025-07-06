use crate::vm::vm::VM;

pub fn op_println(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let value = vm.pop().expect("Stack underflow on op_println");

    match op_type {
        0x01 | 0x02 | 0x07 | 0x08 => println!("{}", value as i64),
        0x03 => println!("{:.10}", f32::from_bits(value as u32)),
        0x04 => println!("{}", f64::from_bits(value)),
        0x05 => println!("{}", if value == 0 { "false" } else {"true"}),
        0x09 => {
            let char_val = std::char::from_u32(value as u32);
            match char_val {
                Some(c) => println!("{}", c),
                None => println!("Invalid char: {}", value),
            }
        }
        _ => {
            println!("Unknown print type: {}", op_type);
        }
    }
}