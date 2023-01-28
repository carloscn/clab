include!("gcd.rs");

// using "`cargo test` to run the unit test"
#[test]
fn test_gcd() {
    assert_eq!(gcd(14, 15), 1);
    assert_eq!(gcd(2*3*11*13*19, 3*7*11*13*19), 3 * 11);
}