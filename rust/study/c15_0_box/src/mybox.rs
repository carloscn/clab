use List::{Cons, Nil};
use std::ops::Deref;
use std::prelude;

fn my_box() {
    let b = Box::new(5);
    println!("b = {b}");

    let list = Cons(1,
                     Box::new(Cons(2,
                        Box::new(Cons(3,
                            Box::new(Nil))))));
}

fn hello(name: &str) {
    println!("Hello, {}", name);
}

struct MyBox<T>(T);

impl<T> MyBox<T> {
    fn new(x: T) -> MyBox<T> {
        MyBox(x)
    }
}

impl<T> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_box() {
        my_box();
    }

    #[test]
    fn test_box_deref() {
        let x = 5;
        let y = Box::new(x);

        assert_eq!(x, *y);
    }

    #[test]
    fn test_my_box() {
        let x = 5;
        let y = MyBox::new(x);
        assert_eq!(x, *y);
    }

    #[test]
    fn test_str_box() {
        let m = Box::new(String::from("Rust"));

        hello(&m);
        assert_eq!(m.to_string(), "Rust".to_string());
    }
}

enum List {
    Cons(i32, Box<List>),
    Nil
}