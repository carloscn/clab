pub fn add_two(a: i32) -> i32
{
    a + 2
}

pub fn greeting(name:&str) -> String
{
    format!("hello! {name}")
}

pub struct Guess {
    value:u32,
}

impl Guess {
    pub fn new(value: u32) -> Guess {
        if value < 10 {
            panic!("Guess value is less then 10.");
        } else if value > 100 {
            panic!("Guess value is more then 100.");
        } else {
            Guess { value }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn it_adds_two() {
        assert_eq!(4, add_two(2));
    }
    #[test]
    fn it_adds_three() {
        assert_ne!(3, add_two(2));
    }
    #[test]
    fn it_adds_five() {
        assert!(3 == add_two(2));
    }

    #[test]
    fn test_greeting() {
        let result = greeting("carlos");
        assert!(result.contains("carlos"), "name contains '{}'", "carlos");
    }

    #[test]
    #[should_panic]
    fn test_guess_panic() {
        Guess::new(200);
    }

    #[test]
    #[should_panic(expected = "Guess value is more then")]
    fn test_guess_panic_exp() {
        Guess::new(5);
    }

    
    #[test]
    fn test_use_result() -> Result<(), String>{
        if 2 + 5 == 4 {
            Ok(())
        } else {
            Err(String::from("two plus does not equel four!"))
        }
    }

    // cargo test -- --ignored
    #[test]
    #[ignore]
    fn test_ignore() {
        assert!(0 != 1);
    }

}