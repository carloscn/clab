
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

fn float_type() {
    let a = -1.5625;
    let b = 2.;
    let c = 0.25;
    let d = 1e4;
    let e = 40f32;
    let f = 9.109_383_56e-31_f64;   // 9.10938356 × 10^–31

    assert_eq!(5f32.sqrt() * 5f32.sqrt(), 5.);
    assert_eq!(-1.01f64.floor(), -1.0);
    assert!((-1./std::f32::INFINITY).is_sign_negative());

    //println!("{}", (2.0).sqrt());     // this is error using
    println!("{}", (2.0_f64).sqrt());
    println!("{}", f64::sqrt(2.0));
}

fn bool_type() {
    // error use
    let x = 1;
    // if x {
    //     println!("error use");
    // }

    if x == 1 {
        println!("correct using bool");
    }

    assert_eq!(false as i32, 0);
    assert_eq!(true as i32, 1);

}

// character type char represents a single Unicode character, as a 32-bit value.
// string represents its text as a sequence of UTF-8 bytes,
fn char_type() {
    // character’s code point is in the range U+0000 to U+007F
    assert_eq!('*', '\x2A');
    assert_eq!('\u{CA0}' ,'ಠ');

    assert_eq!('*' as i32, 42);
    assert_eq!('ಠ' as u16, 0xca0);
    assert_eq!('ಠ' as i8, -0x60); // U+0CA0 truncated to eight bits, signed

    assert_eq!('*'.is_alphabetic(), false);
    assert_eq!('β'.is_alphabetic(), true);
    assert_eq!('8'.to_digit(10), Some(8));
    assert_eq!('ಠ'.len_utf8(), 3);
    assert_eq!(std::char::from_digit(2, 10), Some('2'));
}

// tuples as return value;
fn my_split(name: &str, mid: usize) -> (&str, &str)
{
    let (head, tail) = name.split_at(mid);

    (head, tail)
}

// tuples as input arg
fn my_split_in(name: &str, bounds: (usize, usize)) -> (&str, &str, &str, &str)
{
    let (a, b) = name.split_at(bounds.0);
    let (c, d) = b.split_at(bounds.1);

    (a, b, c, d)
}

fn my_test_return(a1:  usize, a2:  usize) -> (usize, usize)
{
    let a3 = a1;
    let a4 = a2;

    (a3, a4)
}

fn my_swap<T>(x: &mut T, y: &mut T) -> ()
{
    std::mem::swap(x, y);
}

fn tuples_type() {
    let mut x = ("tuples_type", 1985_u16, '*', 0xf_f_ff_fff_f_usize);
    println!("{}", x.0);
    println!("{}", x.1);
    println!("{}", x.2);
    println!("{}", x.3);

    let input = "I see the eigenvalue in thine eye";
    let (c, d) = my_split(input, 21);
    println!("{}", c);
    println!("{}", d);

    let e = (5, 7);
    let k = my_split_in(input, e);

    println!("{}", k.0);
    println!("{}", k.1);
    println!("{}", k.2);
    println!("{}", k.3);

    let mut f = 9;
    let mut g = 1;
    my_swap(&mut f, &mut g);
    println!("{}", f);
    println!("{}", g);

    let mut d = my_test_return(1, 5);
    d.0 = 7;
    d.1 = 8;
}

fn main() {
    inte_type();
    float_type();
    bool_type();
    char_type();
    tuples_type();
    println!("Hello, world!");
}
