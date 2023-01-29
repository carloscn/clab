
fn inte_type() {
    let big_val = std::i32::MAX;
    println!("bit_val is {}", big_val);

    let x = big_val.wrapping_add(1);
    println!("bit_val is {}", x);

    let a = 45u8;
    let b = 17892isize;

    let c = 0x8000u32;
    let d = 0o457;
    let e = 0b100010010001;
    let f = 0b1000_1001_0001isize;
    let g = 127_u8;
    let h = b'A';
    let i = b'\'';
    let j = b'\\';
    let k = b'\n';
    let l = b'\t';
    let m = b'\r';
    let n = 42_i8;
    let o = 42_i16;
    let p = 42_i32;
    let q = 42_i64;
    let r = 42_u8;
    let s = 42_u16;
    let t = 42_u32;
    let u = 42_u64;
    let v = 0xf_f_ff_fff_f_usize;
    let w = '*';

    assert_eq!(10_i8        as u16,     10_u16);
    assert_eq!(2525_u16     as i16,     2525_i16);
    assert_eq!(-1_i16       as i32,     -1_i32);
    assert_eq!(65535_u16    as i32,     65535_i32);
    assert_eq!(-1_i8        as u8,      255_u8);
    assert_eq!(255_u8       as i8,      -1_i8);

    assert_eq!(2u16.pow(4) as i16, 16_i16);
    assert_eq!((-4_i32).abs(), 4);
    assert_eq!(u8::count_ones(0b101001), 3);
    assert_eq!(0b101001u8.count_ones(), 3);
}


fn main() {
    inte_type();
    println!("Hello, world!");
}
