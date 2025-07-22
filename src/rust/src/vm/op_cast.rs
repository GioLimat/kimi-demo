use crate::vm::vm::VM;

enum Value {
    I8(i8), I16(i16), I32(i32), I64(i64),
    U8(u8), U16(u16), U32(u32), U64(u64),
    F32(f32), F64(f64),
    Bool(bool), Char(u32),
}

macro_rules! clamp_to_f32 {
    ($f:expr) => {{
        if $f.is_nan()                    { f32::NAN }
        else if $f > f32::MAX as f64      { f32::INFINITY }
        else if $f < f32::MIN as f64      { f32::NEG_INFINITY }
        else                               { $f as f32 }
    }};
}
macro_rules! clamp_signed {
    ($f:expr, $t:ty) => {{
        if $f.is_nan()      { 0 }
        else if $f > <$t>::MAX as f64 { <$t>::MAX }
        else if $f < <$t>::MIN as f64 { <$t>::MIN }
        else                           { $f.trunc() as $t }
    }};
}

macro_rules! clamp_unsigned {
    ($f:expr, $t:ty) => {{
        if $f.is_nan() || $f <= 0.0       { 0 }
        else if $f > <$t>::MAX as f64    { <$t>::MAX }
        else                              { $f.trunc() as $t }
    }};
}

impl Value {
    fn from_raw(code: u8, raw: u64) -> Self {
        match code {
            0x07 => Value::I8(raw as i8),
            0x08 => Value::I16(raw as i16),
            0x01 => Value::I32(raw as i32),
            0x02 => Value::I64(raw as i64),
            0x0C => Value::U8(raw as u8),
            0x0D => Value::U16(raw as u16),
            0x0E => Value::U32(raw as u32),
            0x0F => Value::U64(raw),
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
            (0x0C, Value::U8(x))   => x as u64,
            (0x0D, Value::U16(x))  => x as u64,
            (0x0E, Value::U32(x))  => x as u64,
            (0x0F, Value::U64(x))  => x,
            (0x03, Value::F32(x))  => x.to_bits() as u64,
            (0x04, Value::F64(x))  => x.to_bits(),
            (0x05, Value::Bool(x)) => x as u64,
            (0x09, Value::Char(c)) => c as u64,
            (to, val) => {
                let f = val.to_f64();
                return match to {
                    0x07 => clamp_signed!(f, i8)   as u64,
                    0x08 => clamp_signed!(f, i16)  as u64,
                    0x01 => clamp_signed!(f, i32)  as u64,
                    0x02 => clamp_signed!(f, i64)  as u64,
                    0x0C => clamp_unsigned!(f, u8) as u64,
                    0x0D => clamp_unsigned!(f, u16) as u64,
                    0x0E => clamp_unsigned!(f, u32) as u64,
                    0x0F => clamp_unsigned!(f, u64),
                    0x03 => clamp_to_f32!(f).to_bits() as u64,
                    0x04 => f.to_bits(),
                    0x05 => (f != 0.0) as u64,
                    0x09 => f.trunc() as u64,
                    _    => panic!("Unknown target type {:02X}", to),
                };
            }
        }
    }

    fn to_f64(&self) -> f64 {
        match *self {
            Value::I8(x)   => x  as f64,
            Value::I16(x)  => x  as f64,
            Value::I32(x)  => x  as f64,
            Value::I64(x)  => x  as f64,
            Value::U8(x)   => x  as f64,
            Value::U16(x)  => x  as f64,
            Value::U32(x)  => x  as f64,
            Value::U64(x)  => x  as f64,
            Value::F32(x)  => x  as f64,
            Value::F64(x)  => x,
            Value::Bool(x) => if x { 1.0 } else { 0.0 },
            Value::Char(c) => c as f64,
        }
    }
}





pub fn op_cast(vm: &mut VM) {
    let to   = vm.bytecode[vm.ip as usize]; vm.ip += 1;
    let from = vm.bytecode[vm.ip as usize]; vm.ip += 1;
    let raw  = vm.pop().expect("stack underflow");
    let val  = Value::from_raw(from, raw);
    vm.push(val.to_raw(to));
}
