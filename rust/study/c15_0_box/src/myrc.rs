use List::{Cons, Nil};
use std::rc::Rc;

enum List {
    Cons(i32, Rc<List>),
    Nil
}

fn my_box() {
    let b = Box::new(5);
    println!("b = {b}");

    let list = Cons(1,
                     Rc::new(Cons(2,
                        Rc::new(Cons(3,
                            Rc::new(Nil))))));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    /*
          b -> 3
                \
                -> 5 -> 10 -> Nil
                /
          c -> 4
     */
    fn test_rc() {
        let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
        println!("the rc count is {}", Rc::strong_count(&a));
        let b = Cons(3, Rc::clone(&a));
        println!("the rc count is {}", Rc::strong_count(&a));
        {
            let c = Cons(4, Rc::clone(&a));
            println!("the rc count is {}", Rc::strong_count(&a));
        }
        println!("the rc count is {}", Rc::strong_count(&a));
    }
}
