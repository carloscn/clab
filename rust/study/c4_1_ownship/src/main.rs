
struct Person {
    name : String,
    birth: i32
}

fn a_f(x : &mut [i32]) {
    println!("a_f x[6] {}", &x[6]);
    x[6] = 8;
}

fn a_g(x : &[i32]) {
    println!("a_g x[6] {}", &x[6]);
}

fn a_h(x : &[i32]) {
    println!("a_h x[6] {}", &x[6]);
}

fn test_move() {
    // wrong case:
    /*
    let s = vec!["duon".to_string(), "ramen".to_string(), "soba".to_string()];
    let t = s;
    let u = t;
    println!("{}", s[1]);   // the s and t memory have already moved to the u;
    */

    let mut composers = Vec::new(); // Vec::new moves to composers
    // to_string moves to person name
    composers.push(Person {name: "Palestrina".to_string(), birth: 1525});

    let mut x: Vec<i32> = vec![10, 20, 30, 40, 50, 78, 99, 99, 99];

    if true {
        a_f(x);
    } else {
        a_g(x);
    }
    a_h(x);
}

fn main() {
    test_move();
    println!("Hello, world!");
}
