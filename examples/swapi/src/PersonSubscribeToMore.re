
let ste = ReasonReact.string;

module GetAllPersons = [%graphql
  {|
    query getAllPersons {
      allPersons {
        id
        age
        name
      }
    }
|}
];

module GetAllPersonsQuery = ReasonApollo.CreateQuery(GetAllPersons);


module NewPerson = [%graphql
{|

  subscription {
    person: Person {
      node {
        name
      }
    } 
  }
|}
];

let newPerson = NewPerson.make();

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self =>
    <GetAllPersonsQuery>
      ...(
           ({result, subscribeToMore}) =>
             <div>
               <h1> ("Persons: " |> ste) </h1>
               (
                 switch (result) {
                 | Error(e) =>
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) => 
                    <ShowLivePersons 
                      persons={response##allPersons} 
                      subscribeToMore={
                        subscribeToMore(
                          ~document=newPerson##query,
                          ()
                        );
                      }
                    />
                 }
               )
             </div>
         )
    </GetAllPersonsQuery>,
};