#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef _CLI_AMI_TABLE_INCLUDE
#    define _CLI_AMI_TABLE_INCLUDE
#    define RET_STRING(_a) #_a
#    define STRINGIFY(_a) RET_STRING(_a)

#    define _CLI_AMI_TABLE_LIST_LOCK(...) CLI_AMI_TABLE_LIST_LOCK(__VA_ARGS__)
#    define _CLI_AMI_TABLE_LIST_ITERATOR(...) CLI_AMI_TABLE_LIST_ITERATOR(__VA_ARGS__)
#    define _CLI_AMI_TABLE_LIST_UNLOCK(...) CLI_AMI_TABLE_LIST_UNLOCK(__VA_ARGS__)

#    define MERGE2_(_a,_b) _a##_b
#    define UNIQUE_VAR(_a,_b) MERGE2_(_a,_b)
#endif
	// =========== Code To be generated by include file ===============
pbx_cli(fd, "\n");
#ifdef CLI_AMI_TABLE_LIST_ITER_TYPE
CLI_AMI_TABLE_LIST_ITER_TYPE *CLI_AMI_TABLE_LIST_ITER_VAR = NULL;
#endif

#ifndef CLI_AMI_TABLE_BEFORE_ITERATION
#    define CLI_AMI_TABLE_BEFORE_ITERATION
#endif
#ifndef CLI_AMI_TABLE_AFTER_ITERATION
#    define CLI_AMI_TABLE_AFTER_ITERATION
#endif

	/* print headers */
int UNIQUE_VAR(table_width_, CLI_AMI_TABLE_NAME) = 0;
const char *UNIQUE_VAR(id, CLI_AMI_TABLE_NAME);
char UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME)[256] = "";

#define CLI_AMI_TABLE_FIELD(_a,_b,_c,_d) UNIQUE_VAR(table_width_,CLI_AMI_TABLE_NAME)=UNIQUE_VAR(table_width_,CLI_AMI_TABLE_NAME) + _c+ 1;
CLI_AMI_TABLE_FIELDS
#undef CLI_AMI_TABLE_FIELD
    if (!s)
{
pbx_cli(fd, "+--- %s %.*s+\n", STRINGIFY(CLI_AMI_TABLE_NAME), UNIQUE_VAR(table_width_, CLI_AMI_TABLE_NAME) - (int)strlen(STRINGIFY(CLI_AMI_TABLE_NAME)) - 4, "------------------------------------------------------------------------------------------------------------------------------------------------------------------");

pbx_cli(fd, "| ");
#define CLI_AMI_TABLE_FIELD(_a,_b,_c,_d) pbx_cli(fd,"%*s ",-_c,#_a);
CLI_AMI_TABLE_FIELDS
#undef CLI_AMI_TABLE_FIELD
    pbx_cli(fd, "|\n");

pbx_cli(fd, "+ ");
#define CLI_AMI_TABLE_FIELD(_a,_b,_c,_d) pbx_cli(fd,"%.*s ",_c,	"==================================================================================================================================================================");
CLI_AMI_TABLE_FIELDS
#undef CLI_AMI_TABLE_FIELD
    pbx_cli(fd, "+\n");
} else {
	astman_append(s, "Event: TableStart\r\n");
	local_total++;
	astman_append(s, "TableName: %s\r\n", STRINGIFY(CLI_AMI_TABLE_NAME));
	local_total++;

	UNIQUE_VAR(id, CLI_AMI_TABLE_NAME) = astman_get_header(m, "ActionID");
	if (!pbx_strlen_zero(UNIQUE_VAR(id, CLI_AMI_TABLE_NAME))) {
		snprintf(UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME), sizeof(UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME)), "ActionID: %s\r\n", UNIQUE_VAR(id, CLI_AMI_TABLE_NAME));
		astman_append(s, "%s\r\n", UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME));
		local_total++;
	}
}

	/* iterator through list */
if (!s) {
#define CLI_AMI_TABLE_FIELD(_a,_b,_c,_d) pbx_cli(fd,STRINGIFY(%*_b%s),-_c,_d," ");
#ifdef CLI_AMI_TABLE_LIST_ITERATOR
	_CLI_AMI_TABLE_LIST_LOCK(CLI_AMI_TABLE_LIST_ITER_HEAD);
	_CLI_AMI_TABLE_LIST_ITERATOR(CLI_AMI_TABLE_LIST_ITER_HEAD, CLI_AMI_TABLE_LIST_ITER_VAR, list) {
#else
	CLI_AMI_TABLE_ITERATOR {
#endif
		CLI_AMI_TABLE_BEFORE_ITERATION pbx_cli(fd, "| ");
		CLI_AMI_TABLE_FIELDS pbx_cli(fd, "|\n");
	CLI_AMI_TABLE_AFTER_ITERATION}
#ifdef CLI_AMI_TABLE_LIST_ITERATOR
	_CLI_AMI_TABLE_LIST_UNLOCK(CLI_AMI_TABLE_LIST_ITER_HEAD);
#endif
#undef CLI_AMI_TABLE_FIELD
} else {
#define CLI_AMI_TABLE_FIELD(_a,_b,_c,_d) astman_append(s, STRINGIFY(%s: %_b\r\n),#_a,_d); local_total++;
#ifdef CLI_AMI_TABLE_LIST_ITERATOR
	_CLI_AMI_TABLE_LIST_LOCK(CLI_AMI_TABLE_LIST_ITER_HEAD);
	_CLI_AMI_TABLE_LIST_ITERATOR(CLI_AMI_TABLE_LIST_ITER_HEAD, CLI_AMI_TABLE_LIST_ITER_VAR, list) {
#else
	CLI_AMI_TABLE_ITERATOR {
#endif
		CLI_AMI_TABLE_BEFORE_ITERATION astman_append(s, "Event: %sEntry\r\n", STRINGIFY(CLI_AMI_TABLE_PER_ENTRY_NAME));

		local_total++;
		astman_append(s, "ChannelType: SCCP\r\n");
		local_total++;
		astman_append(s, "ChannelObjectType: %s\r\n", STRINGIFY(CLI_AMI_TABLE_PER_ENTRY_NAME));
		local_total++;
		if (!pbx_strlen_zero(UNIQUE_VAR(id, CLI_AMI_TABLE_NAME))) {
			astman_append(s, "%s", UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME));
		}
		CLI_AMI_TABLE_FIELDS astman_append(s, "\r\n");

		local_total++;
	CLI_AMI_TABLE_AFTER_ITERATION}
#ifdef CLI_AMI_TABLE_LIST_ITERATOR
	_CLI_AMI_TABLE_LIST_UNLOCK(CLI_AMI_TABLE_LIST_ITER_HEAD);
#endif
#undef CLI_AMI_TABLE_FIELD
}

	/* print footer */
if (!s) {
	pbx_cli(fd, "+%.*s+\n", UNIQUE_VAR(table_width_, CLI_AMI_TABLE_NAME) + 1, "------------------------------------------------------------------------------------------------------------------------------------------------------------------");

} else {
	astman_append(s, "Event: TableEnd\r\n");
	local_total++;
	astman_append(s, "TableName: %s\r\n", STRINGIFY(CLI_AMI_TABLE_NAME));
	local_total++;
	if (!pbx_strlen_zero(UNIQUE_VAR(id, CLI_AMI_TABLE_NAME))) {
		astman_append(s, "%s\r\n", UNIQUE_VAR(idtext, CLI_AMI_TABLE_NAME));
		local_total++;
	}
	astman_append(s, "\r\n");
	local_total++;
}

#ifdef CLI_AMI_TABLE_NAME
#    undef CLI_AMI_TABLE_NAME
#endif

#ifdef CLI_AMI_TABLE_PER_ENTRY_NAME
#    undef CLI_AMI_TABLE_PER_ENTRY_NAME
#endif

#ifdef CLI_AMI_TABLE_LIST_ITER_TYPE
#    undef CLI_AMI_TABLE_LIST_ITER_TYPE
#endif

#ifdef CLI_AMI_TABLE_LIST_ITER_VAR
#    undef CLI_AMI_TABLE_LIST_ITER_VAR
#endif

#ifdef CLI_AMI_TABLE_LIST_ITER_HEAD
#    undef CLI_AMI_TABLE_LIST_ITER_HEAD
#endif

#ifdef CLI_AMI_TABLE_LIST_LOCK
#    undef CLI_AMI_TABLE_LIST_LOCK
#endif

#ifdef CLI_AMI_TABLE_LIST_ITERATOR
#    undef CLI_AMI_TABLE_LIST_ITERATOR
#endif

#ifdef CLI_AMI_TABLE_ITERATOR
#    undef CLI_AMI_TABLE_ITERATOR
#endif

#ifdef CLI_AMI_TABLE_BEFORE_ITERATION
#    undef CLI_AMI_TABLE_BEFORE_ITERATION
#endif

#ifdef CLI_AMI_TABLE_AFTER_ITERATION
#    undef CLI_AMI_TABLE_AFTER_ITERATION
#endif

#ifdef CLI_AMI_TABLE_LIST_UNLOCK
#    undef CLI_AMI_TABLE_LIST_UNLOCK
#endif

#ifdef CLI_AMI_TABLE_FIELDS
#    undef CLI_AMI_TABLE_FIELDS
#endif
	// =========== End of Code To be generated by include file ============
#endif                                                                          /* DOXYGEN_SHOULD_SKIP_THIS */
