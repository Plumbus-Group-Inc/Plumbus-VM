
set(PVM_CODE_GEN_SCRIPT ${CMAKE_SOURCE_DIR}/src/decoder-gen.py)
set(PVM_ISA_YAML ${CMAKE_SOURCE_DIR}/isa.yaml)

function(pvm_add_generated TARGET_NAME TEMPLATE_FILE GENERATED_FILE)
  get_filename_component(DIR ${GENERATED_FILE} DIRECTORY)
  message(STATUS "creating: ${DIR}")
  file(MAKE_DIRECTORY ${DIR})

  add_custom_command(
    OUTPUT ${GENERATED_FILE}
    COMMAND ${Python3_EXECUTABLE} ${PVM_CODE_GEN_SCRIPT}
    -y ${PVM_ISA_YAML}
    -j ${TEMPLATE_FILE}
    -o ${GENERATED_FILE}
    DEPENDS ${PVM_CODE_GEN_SCRIPT} ${PVM_ISA_YAML} ${TEMPLATE_FILE}
    COMMENT "Generating ${GENERATED_FILE}"
    VERBATIM
  )

  add_custom_target(
    ${TARGET_NAME}
    DEPENDS ${TEMPLATE_FILE} ${GENERATED_FILE} ${PVM_ISA_YAML}
    COMMENT "Checking if regeneration is required"
  )

endfunction()
