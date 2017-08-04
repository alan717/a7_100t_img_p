          Node* removeDuplicates(Node *head)
          {
              Node* cur=head->next;
              Node* start=head;
            // head=head->next;
            //Write your code here
              
             while(cur!=NULL)
             {
                 if(cur->data!=head->data)
                 {
                     head=head->next;
                     head->data=cur->data;
                     cur=cur->next;
                 }
                 else{
                  cur=cur->next;
                 }
                 
             }
              head->next=NULL;
              return start;
          }