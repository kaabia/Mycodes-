/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_amazon_S3.h"

/* Private macro -------------------------------------------------------------*/
#define STRING_TO_SIGN_SIZE 133
#define DATE_Length         8

/* Private variables ---------------------------------------------------------*/
uint8_t my_string_to_sign[STRING_TO_SIGN_SIZE];
uint8_t  Sha256_content[64];

/* Variables for the Canonical request */
uint8_t canonical_uri[]= "/download_me_bin_1_mo.bin\n"; /* File name for download */
uint8_t file_name_upload[]="/test_upload_xMB_2.bin\n"; /* File name for upload */
#define FILE_NAME_SIZE sizeof(canonical_uri)
#define FILE_NAME_SIZE_UP sizeof(file_name_upload)
uint8_t file_size_for_download =FILE_NAME_SIZE -2;
uint8_t file_size_for_request  = FILE_NAME_SIZE_UP -2;
/* Canonical Request for download */
#define CANONICAL_REQUEST_SIZE (FILE_NAME_SIZE+ 263)
/* Canonical Request for Upload */
#define MY_CANONICAL_SEED_SIZE  (FILE_NAME_SIZE_UP + 351) /* Max Size */

/* Variables for the String to sign */
const uint8_t sts_algo[]   = "AWS4-HMAC-SHA256";
const uint8_t sts_credsc[] = "/eu-west-1/s3/aws4_request\n";
uint8_t sts_canonical_hash[65];

/*My AWS account variables */
const uint8_t Region[] = "eu-west-1";
uint32_t RegionLen = (sizeof(Region)-1);
const uint8_t Service[] = "s3";
uint32_t ServiceLen = (sizeof(Service)-1);
/*AWS Request example */
const uint8_t Request[] = "aws4_request";
uint32_t RequestLen = (sizeof(Request)-1);
/* The Key ID and the Secret Key */
const uint8_t my_key_id[]= "AKIAILPKTE7VYPVGUFTQ";
uint8_t YourSecretAccessKey[] = "AWS4YRjPf5SVxq3T9VsH0hsNRx8JD00jDVqfL4lBA9Sl";
uint32_t YourSecretAccessKeyLen = (sizeof(YourSecretAccessKey)-1);

uint8_t My_Signature[CRL_SHA256_SIZE];
uint8_t My_Signature_encoded[CRL_SHA256_SIZE *2];
int32_t My_SignatureLength = 0;

/* Used to contain the message digest from canonical request */
uint8_t My_MessageDigest[CRL_SHA256_SIZE];
int32_t My_MessageDigestLength = 0;
/* Used to contain the generated date key signed by your secret key */
uint8_t DateKey[CRL_SHA256_SIZE];
int32_t DateKeyLength = 0;
/* Used to contain the generated region key signed by your date key */
uint8_t DateRegionKey[CRL_SHA256_SIZE];
int32_t DateRegionKeyLength = 0;
/* Used to contain the generated region service key signed by your region key */
uint8_t DateRegionServiceKey[CRL_SHA256_SIZE];
int32_t DateRegionServiceKeyLength = 0;
/* Used to contain the generated region signing key signed by your date region service key */
uint8_t SigningKey[CRL_SHA256_SIZE];
int32_t SigningKeyLength = 0;
/* Used to contain the signature obtained by signing the StringToSign with the signing key */
uint8_t Signature[CRL_SHA256_SIZE];
int32_t SignatureLength = 0;

/* Functions prototypes */
static uint8_t *bin2hex(uint8_t *bin, size_t len);
int32_t STM32_SHA256_HMAC_Compute(uint8_t* InputMessage,
                                uint32_t InputMessageLength,
                                uint8_t *HMAC_key,
                                uint32_t HMAC_keyLength,
                                uint8_t *MessageDigest,
                                int32_t* MessageDigestLength);

int32_t STM32_SHA256_HASH_DigestCompute(const uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength);

int32_t STM32_SHA256_HASH_DigestCompute_Accumulated(const uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength,uint32_t Data_size, uint32_t Hash_size);

/* Variables for File Digest */
static FIL file_object;
UINT byte_readed_h;
char* simulated_file_content_packet_digest; /* Variable used to read the eMMC */

/* Extern Variables used for the Signature*/
extern uint8_t Upload_authorization[249];
extern uint8_t Download_authorization[224];
extern uint8_t my_date_please[30];
extern uint8_t x_amazon_time[17];
extern uint8_t my_short_date[9];

uint8_t my_put_canonical_request[MY_CANONICAL_SEED_SIZE];
extern uint8_t authorization[249];

/**
* @brief  Generates the authorization for the Upload Request
* @retval status 0: if okay, 1 if not.
*/
int PUT_Authorization_calculation()
{
  uint8_t my_chunked_authorization[249];
  uint8_t counter = 0;
  uint8_t *test = NULL;
  int32_t status = HASH_SUCCESS;

  /* Remove last line in the message digest */
  /* Get the SHA265 content */

  /* Building the Canonical Request 
  CanonicalRequest =
  HTTPRequestMethod + '\n' +
  CanonicalURI + '\n' +
  CanonicalQueryString + '\n' +
  CanonicalHeaders + '\n' +
  SignedHeaders + '\n' +
  HexEncode(Hash(RequestPayload))   
  */
  
  strcat((char*)my_put_canonical_request,"PUT\n");
  strcat((char*)my_put_canonical_request,(char*)file_name_upload);
  strcat((char*)my_put_canonical_request,"\n");
  strcat((char*)my_put_canonical_request,"date:");
  strcat((char*)my_put_canonical_request,(char*)my_date_please);
  strcat((char*)my_put_canonical_request,"\n");
  strcat((char*)my_put_canonical_request,"host:electronic-uploads.s3.amazonaws.com\n");
  strcat((char*)my_put_canonical_request,"x-amz-content-sha256:");
  /* Remove extra */
  strcat((char*)my_put_canonical_request,(char*) Sha256_content);
  strcat((char*)my_put_canonical_request,"\n");
  strcat((char*)my_put_canonical_request,"x-amz-date:");
  strcat((char*)my_put_canonical_request,(char*)x_amazon_time);
  strcat((char*)my_put_canonical_request,"\n");
  strcat((char*)my_put_canonical_request,"x-amz-storage-class:STANDARD\n");
  strcat((char*)my_put_canonical_request,"\n");
  strcat((char*)my_put_canonical_request,"date;host;x-amz-content-sha256;x-amz-date;x-amz-storage-class\n");
  strcat((char*)my_put_canonical_request,(char*) Sha256_content);
  
  /* Enable CRC clock */
  __CRC_CLK_ENABLE();
  /* Calculate the message digest of the  canonical request */
  My_MessageDigestLength = 0;

  

  status = STM32_SHA256_HASH_DigestCompute((uint8_t*)my_put_canonical_request, sizeof(my_put_canonical_request)-1,
                                           (uint8_t*)My_MessageDigest, &My_MessageDigestLength);
  
  /* Building my string to sign*/
  strcat((char*)my_string_to_sign,(char*)sts_algo);
  strcat((char*)my_string_to_sign,"\n");
  strcat((char*)my_string_to_sign,(char*)x_amazon_time);
  strcat((char*)my_string_to_sign,"\n");
  strcat((char*)my_string_to_sign,(char*)my_short_date); 
  strcat((char*)my_string_to_sign,(char*)sts_credsc);
  
  /* Construct the hashed message digest  */
  test = bin2hex(My_MessageDigest, 64);
  for(counter = 0; counter < 64; counter ++)
  {
    sts_canonical_hash[counter]= test[counter]; 
  }
  strcat((char*)my_string_to_sign,(char*)sts_canonical_hash);
  
  /* Authorization Builder */
  /* Fill Chunked Authorization with Zeros */
  for(counter = 0; counter < 249; counter++)
  {
    my_chunked_authorization[counter] = 0;
  }
  
  strcat((char*)my_chunked_authorization,"authorization: ");
  strcat((char*)my_chunked_authorization,(char*)sts_algo);
  strcat((char*)my_chunked_authorization," Credential=");
  strcat((char*)my_chunked_authorization,(char*)my_key_id); 
  strcat((char*)my_chunked_authorization,"/");
  strcat((char*)my_chunked_authorization,(char*)my_short_date);
  strcat((char*)my_chunked_authorization,"/");
  strcat((char*)my_chunked_authorization,(char*)Region);
  strcat((char*)my_chunked_authorization,"/");
  strcat((char*)my_chunked_authorization,(char*)Service);
  strcat((char*)my_chunked_authorization,"/");   
  strcat((char*)my_chunked_authorization,(char*)Request);
  strcat((char*)my_chunked_authorization,",SignedHeaders=date;host;x-amz-content-sha256;x-amz-date;x-amz-storage-class,Signature=");
  

    /* Following STM32_SHA256_HMAC_Compute functions will  guide to generate the required signature */
  status = STM32_SHA256_HMAC_Compute((uint8_t*)my_short_date,
                                     DATE_Length,
                                     YourSecretAccessKey,
                                     YourSecretAccessKeyLen,
                                     (uint8_t*)DateKey,
                                     &DateKeyLength);
  
  if (status == HASH_SUCCESS)
  {
    
    status = STM32_SHA256_HMAC_Compute((uint8_t*)Region,
                                       RegionLen,
                                       DateKey,
                                       DateKeyLength,
                                       (uint8_t*)DateRegionKey,
                                       &DateRegionKeyLength);
    
    if (status == HASH_SUCCESS)
    {
      status = STM32_SHA256_HMAC_Compute((uint8_t*)Service,
                                         ServiceLen,
                                         DateRegionKey,
                                         DateRegionKeyLength,
                                         (uint8_t*)DateRegionServiceKey,
                                         &DateRegionServiceKeyLength);  
      if (status == HASH_SUCCESS)
      {
        
        status = STM32_SHA256_HMAC_Compute((uint8_t*)Request,
                                           RequestLen,
                                           DateRegionServiceKey,
                                           DateRegionServiceKeyLength,
                                           (uint8_t*)SigningKey,
                                           &SigningKeyLength);
        
        if (status == HASH_SUCCESS)
        {          
          status = STM32_SHA256_HMAC_Compute((uint8_t*)my_string_to_sign,
                                             STRING_TO_SIGN_SIZE,
                                             SigningKey,
                                             SigningKeyLength,
                                             (uint8_t*)My_Signature,
                                             &My_SignatureLength);
          
          
          /* Get the signature and convert it */
            /* Construct the hashed message digest  */
          test = bin2hex(My_Signature, 64);
          for(counter = 0; counter < 64; counter ++)
          {
            My_Signature_encoded[counter]= test[counter]; 
          }          
          strcat((char*)my_chunked_authorization,(char*)My_Signature_encoded);
          
          /* Fill the new authorization*/
          for(uint32_t i = 0; i < 249; i++)
          {
            authorization[i]= my_chunked_authorization[i];
          }
        }        
      }      
    }    
  }
  free(test);
  return status; 
}

/**
* @brief  Generates the authorization for the Upload Request
* @retval status 0: if okay, 1 if not.
*/
int Authorization_generation()
{
  uint8_t counter = 0;
  uint8_t *test = NULL;
  int32_t status = HASH_SUCCESS;
  uint8_t my_canonical_request[CANONICAL_REQUEST_SIZE-1];
  uint8_t my_download_authorization[224];
  
  /* Building the Canonical Request 
  CanonicalRequest =
  HTTPRequestMethod + '\n' +
  CanonicalURI + '\n' +
  CanonicalQueryString + '\n' +
  CanonicalHeaders + '\n' +
  SignedHeaders + '\n' +
  HexEncode(Hash(RequestPayload))   
  */
  /* Empty the canonical request */
  for(counter = 0; counter < (uint8_t)CANONICAL_REQUEST_SIZE; counter ++)
  {
    my_canonical_request[counter]= 0; 
  }
  strcat((char*)my_canonical_request,"GET\n");
  strcat((char*)my_canonical_request,(char*)canonical_uri);
  strcat((char*)my_canonical_request,"\n");
  strcat((char*)my_canonical_request,"host:electronic-uploads.s3.amazonaws.com\n");
  strcat((char*)my_canonical_request,"x-amz-content-sha256:");
  strcat((char*)my_canonical_request,"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855\n");
  strcat((char*)my_canonical_request,"x-amz-date:");
  strcat((char*)my_canonical_request,(char*)x_amazon_time);
  strcat((char*)my_canonical_request,"\n");
  strcat((char*)my_canonical_request,"\n");
  strcat((char*)my_canonical_request,"host;x-amz-content-sha256;x-amz-date\n");
  strcat((char*)my_canonical_request,"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
  
  /* Enable CRC clock */
  __CRC_CLK_ENABLE();
  
    /* Calculate the HASH of your canonical request */
  status = STM32_SHA256_HASH_DigestCompute((uint8_t*)my_canonical_request, sizeof(my_canonical_request)-1,
                                        (uint8_t*)My_MessageDigest, &My_MessageDigestLength);
  
  /* String to sign construction 
  StringToSign =
  Algorithm + \n +
  RequestDateTime + \n +
  CredentialScope + \n +
  HashedCanonicalRequest
  */
  strcat((char*)my_string_to_sign,(char*)sts_algo);
  strcat((char*)my_string_to_sign,"\n");
  strcat((char*)my_string_to_sign,(char*)x_amazon_time);

  
  strcat((char*)my_string_to_sign,"\n");
  strcat((char*)my_string_to_sign,(char*)my_short_date);
  strcat((char*)my_string_to_sign,(char*)sts_credsc);
  /* Construct the hashed message digest  */
  test = bin2hex(My_MessageDigest, 64);
  for(counter = 0; counter < 64; counter ++)
  {
    sts_canonical_hash[counter]= test[counter]; 
  }
  strcat((char*)my_string_to_sign,(char*)sts_canonical_hash);

  /* Authorization Builder */
  
  for(counter = 0; counter < 224; counter++)
  {
    my_download_authorization[counter] = 0;
  }
  strcat((char*)my_download_authorization,"Authorization: ");
  strcat((char*)my_download_authorization,(char*)sts_algo);
  strcat((char*)my_download_authorization," Credential=");
  strcat((char*)my_download_authorization,(char*)my_key_id); 
  strcat((char*)my_download_authorization,"/");
  strcat((char*)my_download_authorization,(char*)my_short_date);
  strcat((char*)my_download_authorization,"/");
  strcat((char*)my_download_authorization,(char*)Region);
  strcat((char*)my_download_authorization,"/");
  strcat((char*)my_download_authorization,(char*)Service);
  strcat((char*)my_download_authorization,"/");   
  strcat((char*)my_download_authorization,(char*)Request);
  strcat((char*)my_download_authorization,",SignedHeaders=host;x-amz-content-sha256;x-amz-date,Signature=");
  
  
  /* Following STM32_SHA256_HMAC_Compute functions will  guide to generate the required signature */
  status = STM32_SHA256_HMAC_Compute((uint8_t*)my_short_date,
                                     DATE_Length,
                                     YourSecretAccessKey,
                                     YourSecretAccessKeyLen,
                                     (uint8_t*)DateKey,
                                     &DateKeyLength);
  
  if (status == HASH_SUCCESS)
  {
    
    status = STM32_SHA256_HMAC_Compute((uint8_t*)Region,
                                       RegionLen,
                                       DateKey,
                                       DateKeyLength,
                                       (uint8_t*)DateRegionKey,
                                       &DateRegionKeyLength);
    
    if (status == HASH_SUCCESS)
    {
      status = STM32_SHA256_HMAC_Compute((uint8_t*)Service,
                                         ServiceLen,
                                         DateRegionKey,
                                         DateRegionKeyLength,
                                         (uint8_t*)DateRegionServiceKey,
                                         &DateRegionServiceKeyLength);  
      if (status == HASH_SUCCESS)
      {
        
        status = STM32_SHA256_HMAC_Compute((uint8_t*)Request,
                                           RequestLen,
                                           DateRegionServiceKey,
                                           DateRegionServiceKeyLength,
                                           (uint8_t*)SigningKey,
                                           &SigningKeyLength);
        
        if (status == HASH_SUCCESS)
        {          
          status = STM32_SHA256_HMAC_Compute((uint8_t*)my_string_to_sign,
                                             STRING_TO_SIGN_SIZE,
                                             SigningKey,
                                             SigningKeyLength,
                                             (uint8_t*)My_Signature,
                                             &My_SignatureLength);
          
          
          /* Get the signature and convert it */
            /* Construct the hashed message digest  */
          test = bin2hex(My_Signature, 64);
          for(counter = 0; counter < 64; counter ++)
          {
            My_Signature_encoded[counter]= test[counter]; 
          }          
          strcat((char*)my_download_authorization,(char*)My_Signature_encoded);
          
          /* Fill the new authorization*/
          for(uint32_t i = 0; i < 224; i++)
          {
            Download_authorization[i]= my_download_authorization[i];
          }
        }        
      }      
    }    
  }
  free(test);
  return status;
}

/**
* @brief  Generates message digest using SHA256 computation
* @retval error_status 0: if okay, 1 if not.
*/
int32_t STM32_SHA256_HASH_DigestCompute(const uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength)
{
  SHA256ctx_stt P_pSHA256ctx;
  uint32_t error_status = HASH_SUCCESS;

  /* Set the size of the desired hash digest */
  P_pSHA256ctx.mTagSize = CRL_SHA256_SIZE;

  /* Set flag field to default value */
  P_pSHA256ctx.mFlags = E_HASH_DEFAULT;

  error_status = SHA256_Init(&P_pSHA256ctx);

  /* check for initialization errors */
  if (error_status == HASH_SUCCESS)
  {
    /* Add data to be hashed */
    error_status = SHA256_Append(&P_pSHA256ctx,
                                 InputMessage,
                                 InputMessageLength);

    if (error_status == HASH_SUCCESS)
    {
      /* retrieve */
      error_status = SHA256_Finish(&P_pSHA256ctx, MessageDigest, MessageDigestLength);
    }
  }

  return error_status;
}

/**
* @brief  Generates message digest using SHA256 computation from a file in the eMMC
* @retval error_status 0: if okay, 1 if not.
*/
int32_t STM32_SHA256_HASH_DigestCompute_Accumulated(const uint8_t* InputMessage, uint32_t InputMessageLength,
                                        uint8_t *MessageDigest, int32_t* MessageDigestLength,uint32_t Data_size, uint32_t Hash_size)
{
  uint32_t cycles = 0;
  cycles = (uint32_t)Data_size/Hash_size;
  uint32_t last_data = 0;
  last_data = Data_size%Hash_size;
  SHA256ctx_stt P_pSHA256ctx;
  uint32_t error_status = HASH_SUCCESS;
 
  /* Set the size of the desired hash digest */
  P_pSHA256ctx.mTagSize = CRL_SHA256_SIZE;
  
  /* Set flag field to default value */
  P_pSHA256ctx.mFlags = E_HASH_DEFAULT;
  
  
  error_status = SHA256_Init(&P_pSHA256ctx); 
  simulated_file_content_packet_digest = malloc(Hash_size);

  /* Read the first byte to append */
  for(uint32_t counter = 0; counter < cycles; counter ++)
  {
    f_read(&file_object,simulated_file_content_packet_digest, Hash_size, &byte_readed_h);
    /* check for initialization errors */
    if ((error_status == HASH_SUCCESS))
    {
      /* Add data to be hashed */
      error_status = SHA256_Append(&P_pSHA256ctx,
                                   (uint8_t const*) simulated_file_content_packet_digest,
                                   Hash_size);
    }
  }
  
  /* if last data */
  if(last_data)
  {
    f_read(&file_object,simulated_file_content_packet_digest, last_data, &byte_readed_h);
    /* check for initialization errors */
    if (error_status == HASH_SUCCESS)
    {
      /* Add data to be hashed */
      error_status = SHA256_Append(&P_pSHA256ctx,
                                   (uint8_t const*)simulated_file_content_packet_digest,
                                   last_data);
    }
  }
  
  if (error_status == HASH_SUCCESS)
  {
    /* retrieve */
    error_status = SHA256_Finish(&P_pSHA256ctx, MessageDigest, MessageDigestLength);
    free(simulated_file_content_packet_digest);
  }
  else
  {
  }
  return error_status;
}

char  length_size[10];
/**
* @brief  Generates message digest using SHA256 computation and get the SHA256 output
* @retval error_status 0: if okay, 1 if not.
*/
void File_to_Digest(uint8_t * file_name)
{
  uint32_t size = 0; /* Size of the file */
  uint32_t hash_size = 1024;
  int32_t MessageDigestLength1 = 0;
  uint8_t * test = NULL;
  uint8_t my_digest[32];
  
  __CRC_CLK_ENABLE();
  
  /* 1-open the file */
  f_open(&file_object, (char const *)file_name, FA_OPEN_EXISTING|FA_READ);
  size=file_object.fsize;

  __BSW_PRINTF("MY File Name = %s\n", (char*)file_name);
  __BSW_PRINTF("MY File size = %d\n", (char*)size);
  sprintf(length_size, "%u",size);
  __BSW_PRINTF("Calculating Message Digest..");
  STM32_SHA256_HASH_DigestCompute_Accumulated((uint8_t*)simulated_file_content_packet_digest, hash_size,
                                                (uint8_t*)my_digest, &MessageDigestLength1, size,hash_size);
  
  /* close*/
  f_close(&file_object);

  /* convert */
  test = bin2hex(my_digest, 64);
  for(uint32_t counter = 0; counter < 64; counter ++)
  {
    Sha256_content[counter]= test[counter]; 
  }
  
  __BSW_PRINTF("My file hash = %s\n", (char*)Sha256_content);  
}
/**
  * @brief  SHA1 HMAC compute example.
  * @param  InputMessage: pointer to input message to be hashed.
  * @param  InputMessageLength: input data message length in byte.
  * @param  HMAC_key: pointer to key used in the HMAC computation
  * @param  HMAC_keyLength: HMAC key length in byte.
  * @param  MessageDigest: pointer to output parameter that will handle message digest
  * @param  MessageDigestLength: pointer to output digest length.
  * @retval error status: can be HASH_SUCCESS if success or one of
  *         HASH_ERR_BAD_PARAMETER, HASH_ERR_BAD_CONTEXT,
  *         HASH_ERR_BAD_OPERATION if error occured.
  */
int32_t STM32_SHA256_HMAC_Compute(uint8_t* InputMessage,
                                uint32_t InputMessageLength,
                                uint8_t *HMAC_key,
                                uint32_t HMAC_keyLength,
                                uint8_t *MessageDigest,
                                int32_t* MessageDigestLength)
{
  HMAC_SHA1ctx_stt HMAC_SHA1ctx;
  uint32_t error_status = HASH_SUCCESS;

  /* Set the size of the desired MAC*/
  HMAC_SHA1ctx.mTagSize = CRL_SHA256_SIZE;

  /* Set flag field to default value */
  HMAC_SHA1ctx.mFlags = E_HASH_DEFAULT;

  /* Set the key pointer in the context*/
  HMAC_SHA1ctx.pmKey = HMAC_key;

  /* Set the size of the key */
  HMAC_SHA1ctx.mKeySize = HMAC_keyLength;

  /* Initialize the context */
  error_status = HMAC_SHA256_Init(&HMAC_SHA1ctx);

  /* check for initialization errors */
  if (error_status == HASH_SUCCESS)
  {
    /* Add data to be hashed */
    error_status = HMAC_SHA256_Append(&HMAC_SHA1ctx,
                                    InputMessage,
                                    InputMessageLength);

    if (error_status == HASH_SUCCESS)
    {
      /* retrieve */
      error_status = HMAC_SHA256_Finish(&HMAC_SHA1ctx, MessageDigest, MessageDigestLength);
    }
  }
  return error_status;
}
                                  
/**
* @brief  Bin to hex conversion
* @retval pointer with the conversion result
*/
static uint8_t *bin2hex(uint8_t *bin, size_t len)
{
    uint8_t   *out;
    size_t  i;
 
    if (bin == NULL || len == 0)
        return NULL;
 
    out = malloc(len*2+1);
    for (i=0; i<len; i++) {  
        out[i*2]   = "0123456789abcdef"[bin[i] >> 4];
        out[i*2+1] = "0123456789abcdef"[bin[i] & 0x0F];
    }
    out[len*2] = '\0';
 
    return out;
}