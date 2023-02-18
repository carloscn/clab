use std::env;
use std::fs;

pub fn test_readfile() {
    let args:Vec<String> = env::args().collect();

    println!("the args is {:?}", args);

    let query = &args[1];
    let filename = &args[2];
    println!("the query is {:?}, file name is {:?}", query, filename);

    let contents = fs::read_to_string(filename).expect("Something went wrong reading the file.");

    println!("with text: \n {}", contents);
}
