use std::fs::File;
use std::io::{ErrorKind,self};
use std::io::Read;
use std::convert::From;
use std::error::Error;

fn my_open_file()
{
    let f = File::open("hello.txt");
    let f = match f {
        Ok(fd) => fd,
        Err(error) => match error.kind() {
            ErrorKind::NotFound => match File::create("hello.txt") {
                Ok(fc) => fc,
                Err(e) => panic!("error create file {:?}", e),
            },
            OtherError => panic!("Error opening the file : {:?}", OtherError),
        }
    };
}

fn my_open_file_2()
{
    let f = File::open("hello2.txt").unwrap();
    println!("my_open_file_2 finish!");
}

fn my_open_file_3()
{
    let f = File::open("hello3.txt").expect("cannot open the file");
    println!("my_open_file_23 finish!");
}

fn my_open_file_4()
{
    let f = File::open("hello4.txt").unwrap_or_else(|err| {
        if err.kind() == ErrorKind::NotFound {
            File::create("hello4.txt").unwrap_or_else(|err| {
                panic!("Error create file {:?}", err);
            })
        } else {
            panic!("Error opening file: {:?}", err);
        }
    });
    println!("my_open_file_4 finish!");
}

fn my_open_file_5() -> Result<String, io::Error> {

    let f = File::open("hello5.txt");
    let mut f = match f {
        Ok(file) => file,
        Err(e) => return Err(e),
    };

    let mut s = String::new();
    let ret = match f.read_to_string(&mut s) {
        Ok(_) => Ok(s),
        Err(e) => Err(e),
    };

    return ret;
}

fn my_open_file_6() -> Result<String, io::Error> {

    let mut f = File::open("hello6.txt")?;
    let mut s = String::new();
    f.read_to_string(&mut s)?;

    return Ok(s);
}

fn my_open_file_7() -> Result<String, io::Error> {

    let mut s = String::new();
    File::open("hello7.txt")?.read_to_string(&mut s)?;

    return Ok(s);
}

fn my_open_file_8() -> Result<(), Box<dyn Error>> {
    let f = File::open("hello8.txt")?;
    return Ok(());
}

pub struct Guess {
    value : i32,
}

impl Guess {
    pub fn new(value : i32) -> Guess {
        if value < 1 || value > 100 {
            panic!("Guess value mut be 1 - 100, current {}", value);
        }

        Guess { value }
    }

    pub fn value(&self) -> i32 {
        self.value
    }
}

fn main() {
    //my_open_file();
    //my_open_file_2();
    //my_open_file_3();
    //my_open_file_4();
    let ret = my_open_file_5();
    match ret {
        Ok(e) => println!("the content is {e}"),
        Err(e) => println!("oepn file failed."),
    }

    let ret = my_open_file_6();
    match ret {
        Ok(e) => println!("the content is {e}"),
        Err(e) => println!("oepn file failed."),
    }

    let ret = my_open_file_7();
    match ret {
        Ok(e) => println!("the content is {e}"),
        Err(e) => println!("oepn file failed."),
    }
    let ret = my_open_file_8();
    match ret {
        Ok(()) => println!("the content is ok"),
        _ => println!("oepn file failed."),
    }

    let v = Guess::new(99);
    println!("the guss value is {}", v.value());


    println!("Hello, world!");
}
