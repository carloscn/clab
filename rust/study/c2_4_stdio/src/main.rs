use std::io::Write;
use std::str::FromStr;

fn gcd(mut n: u64, m: u64) -> u64 {
    assert!(n != 0 && m != 0);

    let mut tm = m;
    while tm != 0 {
        if tm < n {
            let t = tm;
            tm = n;
            n = t;
        }
        tm = tm % n;
    }
    n
}

fn main() {

    let mut numbers = Vec::new(); // the numbers type is `Vec<u64>`

    // stdin
    for arg in std::env::args().skip(1) {
        let args_interator = u64::from_str(&arg).expect("error parsing argument.");
        numbers.push(args_interator);
    }

    // check args len
    if numbers.len() == 0 {
        writeln!(std::io::stderr(), "Usage: gcd <a> <b>").unwrap();
        std::process::exit(-1);
    }

    // real logic
    let mut d = numbers[0];
    for m in &numbers[1..] {
        println!("gcd input number is gcd({}, {})", d, *m);
        d = gcd(d, *m);
    }

    // output
    println!("The greatest common divisor of {:?} is {}", numbers, d);
    println!("Hello, world!");
}
