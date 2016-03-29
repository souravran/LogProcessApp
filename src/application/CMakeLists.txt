#
# main
#------------------------------------------------------------
set(LogProcessor_SRCS
    LogProcessor.cpp
)
add_executable(LogProcessorApp ${LogProcessor_SRCS})
target_link_libraries(LogProcessorApp ${LOG4CPP_LIBRARIES} )


#-------------------------------------------------------------------------------

#
# add directories
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
