use std::env;
use std::fs;

pub fn test_readfile() {
    let args:Vec<String> = env::args().collect();

    println!("the args is {:?}", args);
    let config = Config::new(&args);

    let contents = fs::read_to_string(config.filename).expect("Something went wrong reading the file.");

    println!("with text: \n {}", contents);
}

struct Config {
    query:String,
    filename:String,
}

impl Config {
    pub fn new(args:&[String]) -> Config {
        let query = args[1].clone();
        let filename = args[2].clone();
        Config { query, filename }
    }
}
