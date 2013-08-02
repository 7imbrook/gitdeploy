//
//  main.c
//  liveServer
//
//  Created by Michael Timbrook on 7/20/13.
//  Copyright (c) 2013 Michael Timbrook. All rights reserved.
//

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define false 0
#define true 1

const char *repoLocation = "/Users/michael/Projects/myTestRepo/";

// Prints repo in form "On branch $(branch_name)"
static void print_branch_name(git_repository *repo) {
    int error = 0;
	const char *branch = NULL;
	git_reference *head = NULL;
    error = git_repository_head(&head, repo);
    if (error == GIT_EORPHANEDHEAD || error == GIT_ENOTFOUND) {
        branch = NULL;
        printf("Error in reading repo\n");
    } else {
        branch = git_reference_name(head);
    }
    if (branch != NULL) {
        branch = &branch[11];
        printf("On branch %s\n", branch);
    }
    // Freedom!!!
    git_reference_free(head);
}

// Prints the last commit message from the current HEAD
static void print_head_commit_message(git_repository *repo) {
    int error = 0;
	git_reference *head = NULL;
    error = git_repository_head(&head, repo);
    if (error == GIT_EORPHANEDHEAD || error == GIT_ENOTFOUND) {
        printf("Error in reading repo\n");
        return;
    }
    
    git_commit *commit = NULL;
    git_oid *oid = malloc(sizeof(git_oid));
    
    git_reference_name_to_id(oid, repo, "HEAD");
    
    git_commit_lookup(&commit, repo, oid);
    
    const char *message = git_commit_message(commit);

    printf("%s\n", message);
    
    // Freedom!!
    git_commit_free(commit);
    free(oid);
}

git_repository *_repo = NULL;
const char *deployed_tag;

// Get the latest deployable tag, deployable is are vX.X.0 tags
int checkTags(const char *name, git_oid *oid, void *payload) {
    
    const char *tag = &name[10];
    
    regex_t regex;
    int reti;
    
    if ((reti = regcomp(&regex, "v[0-9].[0-9].0", 0))) {
        printf("Regex failed\n");
    }
    
    if (!regexec(&regex, tag, 0, NULL, 0)) {
        printf("%s is deployable\n", tag);
    }
    
    
    return 0;
}

int main(int argc, const char * argv[])
{
    
    int error;
    if ((error = git_repository_open(&_repo, repoLocation))) {
        printf("Failed to open with error number %d\n", error);
        return 1;
    }
    
    printf("Successfully opened repo\n");
    print_branch_name(_repo);
    printf("Printing last commit:\n  --> ");
    print_head_commit_message(_repo);
    
    // Read tags
    printf("Reading tags...\n");
    git_tag_foreach_cb testcb = &checkTags;
    git_tag_foreach(_repo, testcb,  NULL);
    
    // Freedom!
    git_repository_free(_repo);
}

