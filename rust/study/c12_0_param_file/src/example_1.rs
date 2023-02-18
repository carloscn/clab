use std::env;

pub fn test_args() {
    let args:Vec<String> = env::args().collect();

    println!("the args is {:?}", args);

    let query = &args[1];
    let filename = &args[2];
    println!("the query is {:?}, file name is {:?}", query, filename);
}
