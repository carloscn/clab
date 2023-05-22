use core::panic;
use std::error::Error;
use std::io::{ErrorKind, Write, Read, self};

mod utils_fo {

    use std::fs::{OpenOptions, read};
    use std::f32::consts::E;
    use std::fs::File;
    use core::panic;
    use std::error::Error;
    use std::io::{ErrorKind, Write, Read, self};

    pub fn write_file_ascii(file_name:&String) -> Result<(), io::Error>
    {
        let mut rc:Result<(), io::Error>;
        // open file
        let f = File::open(file_name);
        let mut f = match f {
            Ok(fd) => fd,
            Err(error) => match error.kind() {
                ErrorKind::NotFound => match File::create(file_name) {
                        Ok(fc) => fc,
                        Err(e) => panic!("error create file {:?}", e),
                },
                Others => panic!("Open file {:?}", Others),
            }
        };

        // Write ASCII to a File in Rust
        rc = match f.write("helloworld".to_string().as_bytes()) {
            Ok(_) => Ok(()),
            Err(error) => return Err(error),
        };

        // Optional, you can use the macro write!
        rc = match write!(f, "dsflkshkglhsklg\n") {
            Ok(_) => Ok(()),
            Err(error) => return Err(error),
        };

        return rc;
    }

    pub fn write_file_ascii_append(file_name:&String) -> Result<(), io::Error>
    {
        let mut rc:Result<(), io::Error>;

        let mut f:File = match OpenOptions::new()
                                    .append(true)
                                    .open(file_name) {
            Ok(file) => file,
            Err(error) => match error.kind() {
                ErrorKind::NotFound => match File::create(file_name) {
                    Ok(fd) => {
                        println!("Not found the file, try to create it!");
                        fd
                    },
                    Err(error) => {
                        println!("create file error\n {:?}", error);
                        return Err(error);
                    },
                },
                OthersError => {
                    println!("open file to append failed!\n {:?}", OthersError);
                    return Err(error)
                },

            },
        };

        rc = match f.write("append file hellop\n".as_bytes()) {
            Ok(_) => Ok(()),
            Err(error) => {
                println!("write failed!\n");
                return Err(error)
            },
        };

        rc = match write!(f, "append file hello using write!()\n") {
            Ok(_) => Ok(()),
            Err(error) => {
                println!("write failed!\n");
                return Err(error)
            },
        };

        return rc;
    }

    pub fn write_file_bin(file_name:&str, buf:&Vec<u8>) -> Result<usize, io::Error>
    {
        let mut rc:Result<usize, io::Error> = Ok(0);
        let mut ctx = match File::open(file_name) {
            Ok(file) => file,
            Err(error) => match error.kind() {
                ErrorKind::NotFound => match File::create(file_name) {
                    Ok(file) => file,
                    Err(error) => {
                        println!("The file not found and create it failed!\n {:?}", error);
                        return Err(error);
                    }
                },
                _ => {
                    println!("The file open failed!\n {:?}", error);
                    return Err(error);
                },
            },
        };

        let buf_slice:&[u8] = &buf;
        rc = match ctx.write_all(buf_slice) {
            Ok(_) => Ok(buf_slice.len()),
            Err(error) => {
                println!("write binary failed!");
                return Err(error);
            }
        };

        return rc;
    }

    pub fn write_file_binary_append(file_name:&str, buf:&Vec<u8>) -> Result<usize, io::Error>
    {
        let mut rc:Result<usize, io::Error>;

        let mut ctx = match OpenOptions::new()
                                    .append(true)
                                    .open(file_name) {
            Ok(file) => file,
            Err(err) => match err.kind() {
                ErrorKind::NotFound => match File::create(file_name) {
                    Ok(file) => file,
                    Err(err) => {
                        println!("file create failed, {:?}", err);
                        return Err(err);
                    },
                },
                ErrorOthers => {
                    println!("Open file failed! {:?}", ErrorOthers);
                    return Err(err);
                },
            },
        };

        let buf_slice:&[u8] = &buf;
        rc = match ctx.write_all(buf_slice) {
            Ok(_) => Ok(buf_slice.len()),
            Err(err) => {
                println!("write binary failed {:?}", err);
                return Err(err);
            },
        };

        return rc;
    }

    pub fn read_file_binary(file_name:&str) -> Result<Vec<u8>, io::Error>
    {
        let rc:Result<Vec<u8>, io::Error>;

        let mut ctx = match File::open(file_name) {
            Ok(file) => file,
            Err(err) => {
                println!("read file error! {:?}", err);
                return Err(err);
            },
        };

        let mut ret_vec:Vec<u8> = vec![];

        ctx.read_to_end(&mut ret_vec).unwrap();
        rc = Ok(ret_vec);

        return rc;
    }

    pub fn read_file_ascii(file_name:&str) -> Result<String, io::Error>
    {
        let rc:Result<String, io::Error>;

        let mut ctx = match File::open(file_name) {
            Ok(file) => file,
            Err(err) => {
                println!("file open error! {:?}", err);
                return Err(err);
            },
        };

        let mut read_str:String = String::new();
        ctx.read_to_string(&mut read_str).unwrap();

        rc = Ok(read_str);

        return rc;
    }
}

mod utils_dir {
    use std::path::Path;
    use core::panic;
    use std::error::Error;
    use std::io::{ErrorKind, Write, Read, self};

    pub fn check_path_exist(dir_name:&str) -> Result<bool, io::Error>
    {
        let mut rc:Result<bool, io::Error>;

        let target_path = Path::new(dir_name);
        if target_path.exists() {
            rc = Ok(true);
        } else {
            rc = Ok(false);
        }

        return rc;
    }

    pub fn check_path_is_dir(dir_name:&str) -> Result<bool, io::Error>
    {
        let target_path = Path::new(dir_name);
        return Ok(target_path.exists());
    }
}

fn main() {

    // let mut rc = utils_fo::write_file_ascii(&"hello.txt".to_string());

    let mut rc: Result<(), io::Error> = utils_fo::write_file_ascii_append(&"hello.txt".to_string());

    let input = vec![1u8, 2,8,9,4,1,2];
    let rc = utils_fo::write_file_bin("hello.bin", &input);

    let rc = utils_fo::read_file_binary("hello.bin").unwrap_or_else(|err| {
        panic!("read failed\n");
    });

    println!("read result is : {:?}", rc);

    let rc = utils_fo::read_file_ascii("hello.txt").unwrap();
    println!("read result ascii is {:?}", rc);
}
