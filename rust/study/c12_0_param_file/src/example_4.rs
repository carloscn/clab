use std::env;
use std::fs;
use std::process;

pub fn test_readfile() {
    let args:Vec<String> = env::args().collect();

    println!("the args is {:?}", args);
    let config = Config::new(&args).unwrap_or_else(|err| {
        eprintln!("Problem parsing arguments: {}", err);
        process::exit(1);
    });

    let contents = fs::read_to_string(config.filename).expect("Something went wrong reading the file.");

    println!("with text: \n {}", contents);
}

struct Config {
    query:String,
    filename:String,
}

impl Config {
    pub fn new(args:&[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("not enough for parameters");
        }
        let query = args[1].clone();
        let filename = args[2].clone();
        Ok(Config { query, filename })
    }
}
