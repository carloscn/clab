extern crate mbedtls_sys;
extern crate openssl_sys;

use mbedtls_sys::md5_context;
use mbedtls_sys::md5_finish;
use mbedtls_sys::md5_init;
use mbedtls_sys::md5_starts;
use mbedtls_sys::md5_update;

use mbedtls_sys::MD_SHA384;
use mbedtls_sys::md_free;
use mbedtls_sys::md_hmac_finish;
use mbedtls_sys::md_hmac_starts;
use mbedtls_sys::md_hmac_update;
use mbedtls_sys::md_info_from_type;
use mbedtls_sys::md_info_t;
use mbedtls_sys::md_context_t;
use mbedtls_sys::md_init;
use mbedtls_sys::md_setup;

use openssl_sys::DLT_NULL;
use openssl_sys::EVP_DigestFinal;
use openssl_sys::EVP_DigestFinal_ex;
use openssl_sys::EVP_DigestInit_ex;
use openssl_sys::EVP_DigestUpdate;
use openssl_sys::EVP_MD;
use openssl_sys::EVP_MD_CTX_free;
use openssl_sys::EVP_MD_CTX_new;
use openssl_sys::EVP_md5;
use openssl_sys::EVP_MD_CTX;
use openssl_sys::c_void;

pub fn openssl_evp_md5(input:&Vec<u8>, output:&mut Vec<u8>) -> Result<i32, String>
{
    if input.is_empty() {
        return Ok(0);
    }

    let mut ret:i32 = 0;
    let mut ctx:*mut EVP_MD_CTX;
    let mut olen:u32 = 0;
    let err_str:String;

    unsafe {
        ctx = EVP_MD_CTX_new();

        ret = EVP_DigestInit_ex(ctx, EVP_md5(), std::ptr::null_mut());
        if 1 != ret {
            err_str = String::from("init failed\n");
            EVP_MD_CTX_free(ctx);
            return Err(err_str);
        }

        ret = EVP_DigestUpdate(ctx, input.as_ptr() as _, input.len());
        if 1 != ret {
            err_str = String::from("update failed\n");
            EVP_MD_CTX_free(ctx);
            return Err(err_str);
        }

        ret = EVP_DigestFinal_ex(ctx, output.as_mut_ptr() as _, &mut olen);
        if 1 != ret {
            err_str = String::from("finish failed\n");
            EVP_MD_CTX_free(ctx);
            return Err(err_str);
        }

        EVP_MD_CTX_free(ctx);
    }

    return Ok(ret);
}

pub fn mbedtls_md5(input:&Vec<u8>, output:&mut Vec<u8>) -> Result<i32, String>
{
    let mut ctx:md5_context = Default::default();

    unsafe {
        md5_init(&mut ctx);
        md5_starts(&mut ctx);
        md5_update(&mut ctx, input.as_ptr(), input.len());
        md5_finish(&mut ctx, output.as_mut_ptr());
    }

    return Ok(0);
}

pub fn mbedtls_hmac_sha384(key:&Vec<u8>,
                           input:&Vec<u8>,
                           output:&mut Vec<u8>)
                           -> Result<i32, String>
{
    let info:*const md_info_t;
    let mut ctx:md_context_t = Default::default();
    let err_str: String;
    let mut ret:i32;

    unsafe {
        md_init(&mut ctx);
        info = md_info_from_type(MD_SHA384);

        ret = md_setup(&mut ctx, info, 1);
        if 0 != ret {
            err_str = String::from("md setup failed\n");
            return Err(err_str);
        }

        ret = md_hmac_starts(&mut ctx, key.as_ptr(), key.len());
        if 0 != ret {
            err_str = String::from("md_hmac_starts failed\n");
            md_free(&mut ctx);
            return Err(err_str);
        }

        ret = md_hmac_update(&mut ctx, input.as_ptr(), input.len());
        if 0 != ret {
            err_str = String::from("md_hmac_update failed\n");
            md_free(&mut ctx);
            return Err(err_str);
        }

        ret = md_hmac_finish(&mut ctx, output.as_mut_ptr());
        if 0 != ret {
            err_str = String::from("md_hmac_finish failed\n");
            md_free(&mut ctx);
            return Err(err_str);
        }

        md_free(&mut ctx);
    }

    return Ok(ret);
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_mbedtls_md5() {
        let in_vec:Vec<u8> = vec!['a' as u8, 'b' as u8, 'c' as u8, 'd' as u8, 'e' as u8, 'f' as u8];
        let mut output:Vec<u8> = vec![0;16];
        let ret = mbedtls_md5(&in_vec, &mut output).unwrap();

        print!("mbedtls MD5 is : ");
        for e in &output {
            print!("{:02X}", *e);
        }
        println!("");
    }

    #[test]
    fn test_openssl_md5() {
        let in_vec:Vec<u8> = vec!['a' as u8, 'b' as u8, 'c' as u8, 'd' as u8, 'e' as u8, 'f' as u8];
        let mut output:Vec<u8> = vec![0;16];
        let ret = openssl_evp_md5(&in_vec, &mut output).unwrap();

        print!("openssl MD5 is : ");
        for e in &output {
            print!("{:02X}", *e);
        }
        println!("");
    }

    #[test]
    fn test_hmac() {
        let key:Vec<u8> = vec![1,5,8,8,9,5,6,204,5,4,8,0,0,0,0];
        let input:Vec<u8> = vec![1,5,8,8,9,5,6,204,5,4,8,0,0,0,0];
        let mut output:Vec<u8> = vec![0;48];
        let ret = mbedtls_hmac_sha384(&key, &input, &mut output).unwrap();

        print!("mbedtls HMAC is : ");
        for e in &output {
            print!("{:02X}", *e);
        }
        println!("");
    }
}

fn main() {

}
