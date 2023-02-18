mod example_1;
mod example_2;
mod example_3;
mod example_4;
mod example_5;

use std::env;
use std::process;

use c12_0_param_file::Config;

fn main() {
    //example_1::test_args();
    //example_2::test_readfile();
    //example_3::test_readfile();
    //example_4::test_readfile();
    //example_5::test_readfile();

    let args:Vec<String> = env::args().collect();

    println!("the args is {:?}", args);
    let config = Config::new(&args).unwrap_or_else(|err| {
        eprintln!("Problem parsing arguments: {}", err);
        process::exit(1);
    });
    if let Err(e) = c12_0_param_file::run(config) {
        eprintln!("Application error: {}", e);
        process::exit(1);
    }
}
