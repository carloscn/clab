fn reference_type() {
    let mut a = 8;
    let b = &mut a;
    *b = 7;
    // wrong case 1:
    // a = 20;  // note, if a is borrowed by b, a cannot be assigned.

    // wrong case 2:
    //b = &mut c; // cannot assign twice to immutable variable
    println!("b is {}", b);
    println!("a is {}", a);

    let d = 9;
    let c = &d;

    println!("d is {}", d);
    println!("c is {}", c);

    let e = 9;
    let mut f = &e;
    // wrong case 1:
    // *f = 20;
    let g = 20;
    f = &g;

    println!("e is {}", e);
    println!("g is {}", g);
    println!("f is {}", f);

    let h = 9;
    // wrong case 1: because the h is not mut.
    // let i = &mut h;
    let i = &h;
    println!("h is {}", h);
    println!("i is {}", i);
}

fn box_type() {
    let t = (12, "egg");
    let b = Box::new(t); // Box<(i32, &str)>
    // when the funtion is returned, the box will free autoly.
}

fn array_trans_real_array(mut arr:[u32;6])
{
    for val in arr.iter() {
        println!("real val is {}", val);
    }
    arr[1] = 999; // cannot change the original array.
}

fn array_trans_reference_array(arr: &mut [u32;6]) {
    for val in arr.iter() {
        println!("ref val is {}", val);
    }
    arr[1] = 999; // will change the original array.
}

fn array_type() {
    let mut array_1 : [u32; 6] = [1,2,3,4,5,6];
    let array_2 = ["hello", "myname", "is", "carlos"];
    let mut array_3 = [0; 10000]; // a array contains 10000 zeros.

    assert_eq!(array_1[3], 4u32);
    assert_eq!(array_2.len(), 4);

    for i in 0..100 {
        array_3[i] = i;
    }

    for i in 0..105 {
        print!("{},", array_3[i]);
    }
    println!("");

    // using iter()
    for val in array_2.iter() {
        println!("{}",val);
    }

    let mut chaos = [5_u32, 1 , 4 , 3 , 2];
    chaos.sort();
    assert_eq!(chaos, [1_u32, 2, 3, 4, 5]);

    array_trans_real_array(array_1);
    for val in array_1.iter() {
        println!("main val is {}", val);
    }
    array_trans_reference_array(&mut array_1);
    for val in array_1.iter() {
        println!("main val is {}", val);
    }
    array_1[1] = 2;
    // fold : (x,y)
    // x is inital value
    // y is |a, b| a+b that interprets as all the elements suming.
    println!("the count of element is {}", array_1.iter().fold(0, |n, _| n+1));
    println!("the sum of element is {}", array_1.iter().fold(0, |n, e| n+e));
    println!("the multiple of element is {}", array_1.iter().fold(1, |n, e| n*e));
}

fn vector_type() {
    // using macro
    let mut v = vec![1,2,1,2];
    assert_eq!(v.iter().fold(1, |a, b| a * b), 4);
    v.push(2);
    assert_eq!(v.iter().fold(1, |a, b| a * b), 8);
    v.push(3);
    assert_eq!(v.iter().fold(1, |a, b| a * b), 24);
    v.pop();
    assert_eq!(v.iter().fold(1, |a, b| a * b), 8);

    // using the new
    let mut s = Vec::new();
    s.push("My ");
    s.push("Name ");
    s.push("is ");
    s.push("Carlos.");
    assert_eq!(s, vec!["My ", "Name ", "is ", "Carlos."]);

    // using the collect
    let q: Vec<i32> = (0..5).collect();
    let c = &v[3];
    assert_eq!(*c, 2);

    let d: Option<&i32> = q.get(3);
    println!("the d is {:?}", d);

    let mut r = vec![1,2,3,4,5,6];
    r.reverse();
    assert_eq!(r, [6,5,4,3,2,1]);

    // using the capacity !!! if the capacity is knew by you at first, please use it rather then Vec::new.
    let mut p = Vec::with_capacity(5);
    assert_eq!(p.len(), 0);
    assert_eq!(p.capacity(), 5);
    p.push(5);
    p.push(10);
    assert_eq!(p.len(), 2);
    assert_eq!(p.capacity(), 5);
    p.push(5);
    p.push(6);
    p.push(7);
    p.push(8);
    assert_eq!(p.len(), 6);
    // over the capacity, the resize is 2*n
    assert_eq!(p.capacity(), 2 * 5);

    let mut k = vec![0,1,2,3];
    k.insert(3, 8);
    assert_eq!(k, vec![0,1,2,8,3]);
    k.remove(1);
    assert_eq!(k, vec![0,2,8,3]);
    assert_eq!(k.pop(), Some(3));
}

fn my_print(n: &[f64]) {
    for elt in n {
        println!("{}", elt);
    }
    print!("\n\n");
}

fn slice_type() {
    // define a vector on heap
    let v: Vec<f64> = vec![0.0, 0.707, 1.0, 0.707];
    // define an array on stack
    let a: [f64; 4] = [0.0, -0.707, -1.0, -0.707];

    let slice_v: &[f64] = &v;
    let slice_a: &[f64] = &a;

    my_print(slice_v); // works on vectors
    my_print(slice_a); // works on arrays

    // we can also use that
    my_print(&v[0..2]);
    my_print(&a[2..]);
    my_print(&slice_a[1..3]);
}

fn string_type() {
    // the a is a pointer only.
    // the string is stored in the pre-reserved readonly memory.
    let a = "\"Ouch!\" said the well.\n";
    print!("{}", a);
    // also print string directly.
    print!("my name is carlos\n");
    print!("hello, hello, hello, this is the, and \
            hello. \
            \n");

    // raw string
    let b = r"C:\Program Files\App";
    //let c = Regex::new(r"\d+(\.\d+)*");
    println!(r###"
            This raw string started with 'r###"'.
            Therefore it does not end until we reach a quote mark ('"')
            followed immediately by three pound signs ('###'):
            "###);

    // byte strings: every char is u8 rather than unicode text
    let c = b"GET"; // &[u8; 3] array.
    assert_eq!(c, &[b'G', b'E', b'T']);

    let d = a.to_string(); // the a is in readonly reserved region,
                           // the to_string copy it to heap.
                           // the d is stack pointer that links the heap.
    let d1 = &d[1..];
    print!("{}", d1);

    // The wrong case:
    // no &mut for string
    // let d2 = &mut d[5..];
    // cannot be indexed.
    // d2[3] = b'#';
    // the index shall be [a..b], but its value cannot be changed.
    let d2 = &d[8..9];
    print!("d2 is {}\n", d2);

    let mut e = String::new();
    e = a.to_string();
    println!("e is {}", e);

    let f = String::from(a);
    println!("f is {}", f);

    // update string
    // using push_str
    e.push_str(" add frist ");
    e.push_str(&f);
    e.push_str(" add last ");
    println!("e is {}", e);

    // using +
    let mut g = String::from("Hello ");
    let g1 = "World!";
    let g2 = g + &g1;
    println!("g2 is {}. ", g2);

    // using format
    let s1 = String::from("C");
    let s2 = String::from("is");
    let s3 = String::from("a");
    let s4 = String::from("programming");
    let s5 = String::from("language.");
    let s = format!("{} {} {} {} {}",s1,s2,s3,s4,s5);
    println!("s is {}",s);

    // iter chars
    let s = "My name is Carlos\n";
    for i in s.chars() {
        print!("{}, ", i);
    }

    let k = vec!["veni", "vidi", "vici"];
    assert_eq!(k.concat(), "venividivici");
    assert_eq!(k.join(", "), "veni, vidi, vici");

    let q = "ONE";
    assert!(q.to_lowercase() == "one");
    assert!("peanut".contains("nut"));
    assert_eq!("hello_eye".replace("eye", "noise"), "hello_noise");
    assert_eq!("  clean\n".trim(), "clean");

    for word in "veni, vidi, vici".split(", ") {
        assert!(word.starts_with("v"));
    }
}

fn main() {
    reference_type();
    box_type();
    array_type();
    vector_type();
    slice_type();
    string_type();
    println!("Hello, world!");
}
