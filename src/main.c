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

#define false 0
#define true 1

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
git_reference *_newestTagRef = NULL;

int checkTags(const char *name, git_oid *oid, void *payload) {
    
    char *strFormat = calloc(80, sizeof(char));
    sprintf(strFormat, "/tmp/live/.git/%s", name);
    FILE *sha = fopen(strFormat, "r");
    
    unsigned char hash[41];
    for(int i = 0; i < 40; i++)
        hash[i] = getc(sha);
    hash[40] = '\0';

    printf("%s -> %s\n", hash, name);
    
    // git_oid tagoid;
    // git_oid_fromraw(&tagoid, hash);
    
    // git_commit *commit = NULL;
    
    // git_commit_lookup(&commit, _repo, &tagoid);
    
    // const char *message = git_commit_message(commit);
    // printf("%s", message);
    
    return 0;
}

int main(int argc, const char * argv[])
{
    
    system("rm -rf /tmp/live"); // <<<<< TESTING CODE, NOT FINAL >>>>> //
    
    int error;
    if ((error = git_clone(&_repo, "/Users/michael/Projects/liveServer", "/tmp/live", NULL))) {
        printf("Failed to clone with error %d\n", error);
        return 1;
    }
    
    printf("Successfully cloned repo to /tmp/live\n");
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

