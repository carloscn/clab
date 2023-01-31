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

fn main() {
    reference_type();
    box_type();
    array_type();
    vector_type();
    println!("Hello, world!");
}
