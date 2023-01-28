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