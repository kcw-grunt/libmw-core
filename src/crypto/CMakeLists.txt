set(TARGET_NAME Crypto)

file(GLOB SOURCE_CODE
	"AggSig.cpp"
	"Bulletproofs.cpp"
	"Crypto.cpp"
	"Pedersen.cpp"
	"PublicKeys.cpp"
)

#if(GRINPP_STATIC)
	add_library(${TARGET_NAME} STATIC ${SOURCE_CODE})
#else(GRINPP_STATIC)
#	add_library(${TARGET_NAME} SHARED ${SOURCE_CODE})
#endif(GRINPP_STATIC)

target_compile_definitions(${TARGET_NAME} PRIVATE MW_CRYPTO)

add_dependencies(${TARGET_NAME} crypto_deps secp256k1-zkp Common)
target_link_libraries(${TARGET_NAME} PUBLIC crypto_deps secp256k1-zkp Common)