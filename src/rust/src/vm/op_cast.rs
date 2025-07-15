use crate::vm::vm::VM;

enum Value {
    I8(i8),
    I16(i16),
    I32(i32),
    I64(i64),
    F32(f32),
    F64(f64),
    Bool(bool),
    Char(u32),
}

impl Value {
    fn from_raw(code: u8, raw: u64) -> Self {
        match code {
            0x07 => Value::I8(raw as i8),
            0x08 => Value::I16(raw as i16),
            0x01 => Value::I32(raw as i32),
            0x02 => Value::I64(raw as i64),
            0x03 => Value::F32(f32::from_bits(raw as u32)),
            0x04 => Value::F64(f64::from_bits(raw)),
            0x05 => Value::Bool(raw != 0),
            0x09 => Value::Char(raw as u32),
            _    => panic!("Unknown source type {:02X}", code),
        }
    }

    fn to_raw(self, target: u8) -> u64 {
        match (target, self) {
            (0x07, Value::I8(x))   => x as u64,
            (0x08, Value::I16(x))  => x as u64,
            (0x01, Value::I32(x))  => x as u64,
            (0x02, Value::I64(x))  => x as u64,
            (0x03, Value::F32(x))  => x.to_bits() as u64,
            (0x04, Value::F64(x))  => x.to_bits(),
            (0x05, Value::Bool(x)) => x as u64,
            (0x09, Value::Char(c)) => c as u64,

            (to, val) => {
                let f = val.to_f64();
                match to {
                    0x07 => clamp_f64_to_i8(f) as u64,
                    0x08 => clamp_f64_to_i16(f) as u64,
                    0x01 => clamp_f64_to_i32(f) as u64,
                    0x02 => clamp_f64_to_i64(f) as u64,
                    0x03 => clamp_f64_to_f32(f).to_bits() as u64,
                    0x04 => f.to_bits(),
                    0x05 => (f != 0.0) as u64,
                    0x09 => (f as u32) as u64,
                    _    => panic!("Unknown target type {:02X}", to),
                }
            }
        }
    }

    fn to_f64(&self) -> f64 {
        match self {
            Value::I8(x)   => *x as f64,
            Value::I16(x)  => *x as f64,
            Value::I32(x)  => *x as f64,
            Value::I64(x)  => *x as f64,
            Value::F32(x)  => *x as f64,
            Value::F64(x)  => *x,
            Value::Bool(x) => if *x { 1.0 } else { 0.0 },
            Value::Char(c) => *c as f64,
        }
    }
}

fn clamp_f64_to_i8(f: f64) -> i8 {
    if f.is_nan() { 0 }
    else if f > i8::MAX as f64 { i8::MAX }
    else if f < i8::MIN as f64 { i8::MIN }
    else { f.trunc() as i8 }
}

fn clamp_f64_to_i16(f: f64) -> i16 {
    if f.is_nan() { 0 }
    else if f > i16::MAX as f64 { i16::MAX }
    else if f < i16::MIN as f64 { i16::MIN }
    else { f.trunc() as i16 }
}

fn clamp_f64_to_i32(f: f64) -> i32 {
    if f.is_nan() { 0 }
    else if f > i32::MAX as f64 { i32::MAX }
    else if f < i32::MIN as f64 { i32::MIN }
    else { f.trunc() as i32 }
}

fn clamp_f64_to_i64(f: f64) -> i64 {
    if f.is_nan() { 0 }
    else if f > i64::MAX as f64 { i64::MAX }
    else if f < i64::MIN as f64 { i64::MIN }
    else { f.trunc() as i64 }
}

fn clamp_f64_to_f32(f: f64) -> f32 {
    if f.is_nan() {
        f32::NAN
    } else if f > f32::MAX as f64 {
        f32::INFINITY
    } else if f < f32::MIN as f64 {
        f32::NEG_INFINITY
    } else {
        f as f32
    }
}

pub fn op_cast(vm: &mut VM) {
    let to = vm.bytecode[vm.ip as usize];
    vm.ip += 1;
    let from = vm.bytecode[vm.ip as usize];
    vm.ip += 1;

    let raw = vm.pop().expect("Stack underflow on op_cast");
    
    let val = Value::from_raw(from, raw);
    let casted = val.to_raw(to);
    vm.push(casted);
}

