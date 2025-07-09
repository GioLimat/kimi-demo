use crate::vm::heap_manager::HeapValue;
use crate::vm::vm::VM;

fn format_value(vm: &VM, elem_type: u8, raw: u64) -> String {
    match elem_type {
        0x01 | 0x02 | 0x07 | 0x08 => (raw as i64).to_string(),
        0x03 => format!("{:.10}", f32::from_bits(raw as u32)),
        0x04 => f64::from_bits(raw).to_string(),
        0x05 => if raw == 0 { "false".into() } else { "true".into() },
        0x09 => std::char::from_u32(raw as u32).map(|c| c.to_string()).unwrap_or_else(|| raw.to_string()),
        0x0A => if let HeapValue::String(s) = vm.heap_manager.get(raw).unwrap() {
            format!("\"{}\"", s)
        } else {
            raw.to_string()
        },
        0x0B => format_array(vm, raw),
        _ => raw.to_string(),
    }
}

fn format_array(vm: &VM, id: u64) -> String {
    let (elem_type, _, elements) = vm.heap_manager.get(id).unwrap().as_array().unwrap();
    let items: Vec<String> = elements
        .iter()
        .map(|&raw| format_value(vm, *elem_type, raw))
        .collect();
    format!("[{}]", items.join(", "))
}

pub fn op_println(vm: &mut VM) {
    let op_type = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let value = vm.pop().expect("Stack underflow on op_println");
    match op_type {
        0x01 | 0x02 | 0x07 | 0x08 => println!("{}", value as i64),
        0x03 => println!("{:.10}", f32::from_bits(value as u32)),
        0x04 => println!("{}", f64::from_bits(value)),
        0x05 => println!("{}", if value == 0 { "false" } else { "true" }),
        0x09 => {
            if let Some(c) = std::char::from_u32(value as u32) {
                println!("{}", c);
            } else {
                println!("{}", value);
            }
        }
        0x0A => if let HeapValue::String(s) = vm.heap_manager.get(value).unwrap() {
            println!("\"{}\"", s);
        } else {
            println!("{}", value);
        },
        0x0B => println!("{}", format_array(vm, value)),
        _ => println!("Unknown print type: {}", op_type),
    }
}